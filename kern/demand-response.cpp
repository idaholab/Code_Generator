/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2015 Idaho National Laboratory
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Jeffrey Young <jeffrey.young@inl.gov>
 */

/**
 * \file demand-response.cpp
 * \brief Demand-response Class.
 * \author Jeffrey Young
 * \date 2015
 */

#include "demand-response.h"
#include "generator.h"

#include "utils.h"

DemandResponse::DemandResponse(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime) : Application(type, indice, senderNode, receiverNode, startTime, endTime)
{
  this->SetAppName(std::string("demandResponse_" + this->GetIndice()));
}

DemandResponse::~DemandResponse()
{
}

std::vector<std::string> DemandResponse::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/applications-module.h\"");
  headers.push_back("#include \"ns3/demand-response-application-helper.h\"");

  return headers;
}

std::vector<std::string> DemandResponse::GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

  apps.push_back("");
  apps.push_back("DemandResponseApplicationHelper dr_" + this->GetAppName() + " (iface_ndc_hub_" + this->GetIndice() + ".GetAddress (1), mdmApps_meterDataManagement_" + this->GetIndice() + ");");
  apps.push_back("ApplicationContainer drApps_" + this->GetAppName() + " = dr_" + this->GetAppName() + ".Install (" + this->GetReceiverNode() + ");");
  apps.push_back("drApps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".0));");
  apps.push_back("drApps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  return apps;
}

std::vector<std::string> DemandResponse::GenerateApplicationPython(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

/// Python version

  return apps;

}
