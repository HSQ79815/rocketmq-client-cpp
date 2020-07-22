/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ROCKETMQ_MESSAGE_MESSAGEID_H_
#define ROCKETMQ_MESSAGE_MESSAGEID_H_

#include <cstdlib>

#include "SocketUtil.h"
#include "UtilAll.h"

namespace rocketmq {

class MessageId {
 public:
  MessageId() : MessageId(nullptr, 0) {}
  MessageId(struct sockaddr* address, int64_t offset) : address_(nullptr), offset_(offset) { setAddress(address); }

  MessageId(const MessageId& other) : MessageId(other.address_, other.offset_) {}
  MessageId(MessageId&& other) : address_(other.address_), offset_(other.offset_) { other.address_ = nullptr; }

  virtual ~MessageId() { std::free(address_); }

  MessageId& operator=(const MessageId& other) {
    if (&other != this) {
      setAddress(other.address_);
      this->offset_ = other.offset_;
    }
    return *this;
  }

  const struct sockaddr* getAddress() const { return address_; }
  void setAddress(struct sockaddr* address) { address_ = copySocketAddress(address_, address); }

  int64_t getOffset() const { return offset_; }
  void setOffset(int64_t offset) { offset_ = offset; }

 private:
  struct sockaddr* address_;
  int64_t offset_;
};

}  // namespace rocketmq

#endif  // ROCKETMQ_MESSAGE_MESSAGEID_H_
