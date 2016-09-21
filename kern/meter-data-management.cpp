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
 * \file meter-data-management.cpp
 * \brief Meter-data-management Class.
 * \author Jeffrey Young
 * \date 2015
 */

#include "meter-data-management.h"
#include "generator.h"

#include "utils.h"

MeterDataManagement::MeterDataManagement(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &interval, const size_t &readingTime) : Application(type, indice, senderNode, receiverNode, startTime, endTime)
{
  this->m_interval = interval;
  this->m_readingTime = readingTime;
  this->SetAppName(std::string("meterDataManagement_" + this->GetIndice()));
}

MeterDataManagement::~MeterDataManagement()
{
}

std::string MeterDataManagement::GetInterval()
{
  return utils::integerToString(this->m_interval);
}

void MeterDataManagement::SetInterval(const size_t &interval)
{
  this->m_interval = interval;
}

std::string MeterDataManagement::GetReadingTime()
{
  return utils::integerToString(this->m_readingTime);
}

void MeterDataManagement::SetReadingTime(const size_t &readingTime)
{
  this->m_readingTime = readingTime;
}

std::vector<std::string> MeterDataManagement::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/applications-module.h\"");
  headers.push_back("#include \"ns3/mdm-application-helper.h\"");

  return headers;
}

std::vector<std::string> MeterDataManagement::GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

std::cerr << this->GetIndice() << std::endl;
std::cerr << this->GetAppName() << std::endl;
std::cerr << this->GetSenderNode() << std::endl;
std::cerr << this->GetReceiverNode() << std::endl;
std::cerr << this->GetStartTime() << std::endl;
std::cerr << this->GetEndTime() << std::endl;
std::cerr << this->GetEndTimeNumber() << std::endl;
std::cerr << this->GetApplicationType() << std::endl;
std::cerr << this->GetInterval() << std::endl;
std::cerr << this->GetReadingTime() << std::endl << std::endl;

  apps.push_back("");
  apps.push_back("MeterDataManagementApplicationHelper mdm_" + this->GetAppName() + " (dcApps_dataConcentrator_" + this->GetIndice() + ", iface_ndc_p2p_" + this->GetIndice() + ".GetAddress (1), Seconds (" + this->GetInterval() + ".0), " + this->GetReadingTime() + ".0);");
  apps.push_back("ApplicationContainer mdmApps_" + this->GetAppName() + " = mdm_" + this->GetAppName() + ".Install (" + this->GetReceiverNode() + ");");
  apps.push_back("mdmApps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".0));");
  apps.push_back("mdmApps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  return apps;
}
