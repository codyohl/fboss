/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#pragma once

#include "fboss/agent/ThriftHandler.h"
#include "fboss/agent/hw/sai/diag/DiagShell.h"
#include "fboss/agent/hw/sai/switch/gen-cpp2/SaiCtrl.h"

namespace facebook::fboss {

class DiagCmdServer;
class SaiSwitch;
class StreamingDiagShellServer;

class SaiHandler : public apache::thrift::ServiceHandler<SaiCtrl> {
 public:
  explicit SaiHandler(const SaiSwitch* hw);
  ~SaiHandler() override;
  apache::thrift::ResponseAndServerStream<std::string, std::string>
  startDiagShell() override;
  void produceDiagShellInput(
      std::unique_ptr<std::string> input,
      std::unique_ptr<ClientInformation> client) override;

  void diagCmd(
      fbstring& result,
      std::unique_ptr<fbstring> cmd,
      std::unique_ptr<ClientInformation> client,
      int16_t serverTimeoutMsecs = 0,
      bool bypassFilter = false) override;

  SwitchRunState getHwSwitchRunState() override;

  void getHwFabricReachability(
      std::map<::std::int64_t, ::facebook::fboss::FabricEndpoint>& reachability)
      override;

 private:
  const SaiSwitch* hw_;
  StreamingDiagShellServer diagShell_;
  DiagCmdServer diagCmdServer_;
};

} // namespace facebook::fboss
