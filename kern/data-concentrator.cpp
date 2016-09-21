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
 * \file data-concentrator.cpp
 * \brief Data-concentrator Class.
 * \author Jeffrey Young
 * \date 2015
 */

#include "data-concentrator.h"
#include "generator.h"

#include "utils.h"

DataConcentrator::DataConcentrator(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime) : Application(type, indice, senderNode, receiverNode, startTime, endTime)
{
  this->SetAppName(std::string("dataConcentrator_" + this->GetIndice()));
}

DataConcentrator::~DataConcentrator()
{
}

std::vector<std::string> DataConcentrator::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/applications-module.h\"");
  headers.push_back("#include \"ns3/data-concentrator-helper.h\"");

  return headers;
}

std::vector<std::string> DataConcentrator::GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

  apps.push_back("");
  apps.push_back("DataConcentratorApplicationHelper dc_" + this->GetAppName() + " (clientApps_udpCosem_" + this->GetIndice() + ", iface_ndc_p2p_" + this->GetIndice() + ".GetAddress (1), iface_ndc_p2p_" + this->GetIndice() + ".GetAddress (0));");
  apps.push_back("ApplicationContainer dcApps_" + this->GetAppName() + " = dc_" + this->GetAppName() + ".Install (ap_" + this->GetIndice() + ");");
  apps.push_back("dcApps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".001));");
  apps.push_back("dcApps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  return apps;
}

std::vector<std::string> DataConcentrator::GenerateApplicationPython(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

/// Python version

  return apps;

}
