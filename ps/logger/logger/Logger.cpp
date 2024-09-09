#include "Logger.h"

#include <bit>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <stop_token>
#include <iostream>

namespace {

using namespace std::chrono_literals;

constexpr uint64_t next_pow2(uint64_t x) noexcept {
  return x == 1 ? 1 : 1 << (64 - std::countl_zero(x - 1));
}

void loggerLoop(auto& queue, std::stop_token stopToken, std::function<void(logging::LogMessage const&)> const& logMessageHandler, auto sleepDuration) {
  while (!stopToken.stop_requested()) {
    if (auto msg = queue.pop()) {
      logMessageHandler(*msg);
    } else {
      std::this_thread::sleep_for(sleepDuration);
    }
    
    while (auto msg = queue.pop()) {
      logMessageHandler(*msg);
    }
  }
}

auto toString(std::chrono::system_clock::time_point tp) {
    auto tt = std::chrono::system_clock::to_time_t(tp);
    auto s = std::string(std::ctime(&tt));
    s.pop_back();
    return s;
}

auto messageToString(logging::LogMessage const& msg) {
  return std::format("{}: {}", toString(msg.timestamp), msg.msg);
}

}  // namespace

logging::handlers::HandlerT logging::handlers::createCoutHandler() noexcept {
  return [](logging::LogMessage const& msg) { std::cout << messageToString(msg) << std::endl; };
}

logging::handlers::HandlerT logging::handlers::createFileHandler(std::ofstream& out) {
  return [&out](logging::LogMessage const& msg) { out << messageToString(msg) << std::endl; };
}

logging::Queue::Queue(size_t size) : mSize(next_pow2(size)), mMessages(mSize) {}

void logging::Queue::push(std::string msg) noexcept {
  auto const writeIdx = mask(mNextWrite++);
  auto& [done, slot] = mMessages[writeIdx];
  if (slot.overflow = done) {
    done = false;
    // std::format("Buffer overflow! writeIdx: {}, mNextWrite: {}, mNextRead: {}, size: {}", writeIdx, mNextWrite.load(), mNextRead, mSize);
  }
  slot.timestamp = std::chrono::system_clock::now();
  slot.msg = std::move(msg);
  done = true;
}

std::optional<logging::LogMessage> logging::Queue::pop() noexcept {
  if (mNextRead == mNextWrite) return {};
  auto const numMissed = static_cast<int>(mNextWrite) - static_cast<int>(mNextRead) - static_cast<int>(mSize);
  if (numMissed > 0) {
    mNextRead += numMissed;
    return LogMessage{std::chrono::system_clock::now(), std::format("Missed {} message{}", numMissed, numMissed == 1 ? "" : "s"), false};
  }
  auto readIdx = mask(mNextRead);
  auto& [done, msg] = mMessages[readIdx];
  if (!done) return {};
  done = false;
  ++mNextRead;
  return msg;
}

size_t constexpr logging::Queue::mask(size_t idx) const noexcept {
  return idx & (mSize - 1);
}

logging::Logger::Logger() : Logger(1024, handlers::createCoutHandler(), 1ms) {}

logging::Logger::Logger(size_t queueSize, std::function<void(LogMessage const&)> messageHandler, std::chrono::milliseconds sleepDuration)
    : mQueue(queueSize),
      mHandler([this,
                messageHandler = std::move(messageHandler),
                sleepDuration](std::stop_token stopToken) { loggerLoop(mQueue, stopToken, messageHandler, sleepDuration); }) {}
