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
#ifndef ROCKETMQ_REQUESTCALLBACK_H_
#define ROCKETMQ_REQUESTCALLBACK_H_

#include "MQException.h"
#include "MQMessage.h"

namespace rocketmq {

enum RequestCallbackType { REQUEST_CALLBACK_TYPE_SIMPLE = 0, REQUEST_CALLBACK_TYPE_AUTO_DELETE = 1 };

/**
 * RequestCallback - callback interface for async request
 */
class ROCKETMQCLIENT_API RequestCallback {
 public:
  virtual ~RequestCallback() = default;

  virtual void onSuccess(MQMessage message) = 0;
  virtual void onException(MQException& e) noexcept = 0;

  virtual RequestCallbackType getRequestCallbackType() const { return REQUEST_CALLBACK_TYPE_SIMPLE; }

 public:
  inline void invokeOnSuccess(MQMessage message) {
    auto type = getRequestCallbackType();
    onSuccess(std::move(message));
    if (type == REQUEST_CALLBACK_TYPE_AUTO_DELETE && getRequestCallbackType() == REQUEST_CALLBACK_TYPE_AUTO_DELETE) {
      delete this;
    }
  }

  inline void invokeOnException(MQException& exception) noexcept {
    auto type = getRequestCallbackType();
    onException(exception);
    if (type == REQUEST_CALLBACK_TYPE_AUTO_DELETE && getRequestCallbackType() == REQUEST_CALLBACK_TYPE_AUTO_DELETE) {
      delete this;
    }
  }
};

/**
 * AutoDeleteRequestCallback - callback interface for async request
 *
 * the object of AutoDeleteRequestCallback will be deleted automatically by SDK after invoke callback interface
 */
class ROCKETMQCLIENT_API AutoDeleteRequestCallback : public RequestCallback {
 public:
  RequestCallbackType getRequestCallbackType() const override final { return REQUEST_CALLBACK_TYPE_AUTO_DELETE; }
};

}  // namespace rocketmq

#endif  // ROCKETMQ_REQUESTCALLBACK_H_
