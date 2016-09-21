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
 * \file udp-cosem.cpp
 * \brief Udp-cosem Class, modeled on Udp class.
 * \author Jeffrey Young
 * \date 2015
 */

#include "udp-cosem.h"
#include "generator.h"

#include "utils.h"

UdpCosem::UdpCosem(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &port) : Application(type, indice, senderNode, receiverNode, startTime, endTime)
{
  this->m_port = port;
  this->SetAppName(std::string("udpCosem_" + this->GetIndice()));
  /* default values */
  this->m_packetSize = 1024;
  this->m_maxPacketCount = 1;
  this->m_packetIntervalTime = std::string("1.0");
}

UdpCosem::~UdpCosem()
{
}

size_t UdpCosem::GetPort()
{
  return this->m_port;
}

void UdpCosem::SetPort(const size_t &port)
{
  this->m_port = port;
}

void UdpCosem::SetPacketSize(const size_t &packetSize)
{
  this->m_packetSize = packetSize;
}

size_t UdpCosem::GetPacketSize()
{
  return this->m_packetSize;
}

void UdpCosem::SetMaxPacketCount(const size_t &maxPacketCount)
{
  this->m_maxPacketCount = maxPacketCount;
}

size_t UdpCosem::GetMaxPacketCount()
{
  return this->m_maxPacketCount;
}

void UdpCosem::SetPacketIntervalTime(const std::string &packetIntervalTime)
{
  this->m_packetIntervalTime = packetIntervalTime;
}

std::string UdpCosem::GetPacketIntervalTime()
{
  return this->m_packetIntervalTime;
}

std::vector<std::string> UdpCosem::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/applications-module.h\"");
  headers.push_back("#include \"ns3/udp-cosem-client-helper.h\"");
  headers.push_back("#include \"ns3/udp-cosem-server-helper.h\"");

  return headers;
}

std::vector<std::string> UdpCosem::GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;
  (void)netDeviceContainer;
  (void)numberIntoNetDevice;

  std::string getReceiverNode = this->GetReceiverNode();
  std::string recNode;

  std::string getSenderNode = this->GetSenderNode();
  std::string sendNode;

  apps.push_back("");
  apps.push_back("UdpCosemServerHelper server_" + this->GetAppName() + " (iface_ndc_sta_" + this->GetIndice() + ");");
  
  if (this->GetReceiverNode().find("term_") == 0)
    recNode = "all_hub_" + this->GetIndice() + ".Get(" + getReceiverNode.substr(getReceiverNode.find("_") + 1) + ")";
  else if (this->GetReceiverNode().find("station_") == 0)
    recNode = "all_ap_" + this->GetIndice() + ".Get(" + getReceiverNode.substr(getReceiverNode.find("_") + 1) + ")";

  apps.push_back("ApplicationContainer serverApps_" + this->GetAppName() + " = server_" + this->GetAppName() + ".Install (all_ap_" + this->GetIndice() + ");");

  apps.push_back("serverApps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".0));");
  apps.push_back("serverApps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  apps.push_back("");
  apps.push_back("Time interPacketInterval_" + this->GetAppName() + " = Seconds (" + this->m_packetIntervalTime + ");");
  apps.push_back("");

  apps.push_back("UdpCosemClientHelper client_" + this->GetAppName() + " (serverApps_" + this->GetAppName() + ", iface_ndc_ap_" + this->GetIndice() + ", " + "interPacketInterval_" + this->GetAppName() + ");");

  apps.push_back("ApplicationContainer clientApps_" + this->GetAppName() + " = client_" + this->GetAppName() + ".Install (ap_" + this->GetIndice() + ");");

  if (this->GetSenderNode().find("term_") == 0)
    sendNode = "all_hub_" + this->GetIndice() + ".Get(" + getSenderNode.substr(getSenderNode.find("_") + 1) + ")";
  else if (this->GetSenderNode().find("station_") == 0)
    sendNode = "all_ap_" + this->GetIndice() + ".Get(" + getSenderNode.substr(getSenderNode.find("_") + 1) + ")";

  apps.push_back("clientApps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".0));");
  apps.push_back("clientApps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  return apps;
}
