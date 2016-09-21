/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Strasbourg
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
 * Author: Pierre Weiss <3weissp@gmail.com>
 */

/**
 * \file udp.cpp
 * \brief Udp Class, modeled on Udp-echo class.
 * \author Jeffrey Young
 * \date 2015
 */

#include "udp.h"
#include "generator.h"

#include "utils.h"

Udp::Udp(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &port) : Application(type, indice, senderNode, receiverNode, startTime, endTime)
{
  this->m_port = port;
  this->SetAppName(std::string("udp_" + this->GetIndice()));
  /* default values */
  this->m_packetSize = 1024;
  this->m_maxPacketCount = 1;
  this->m_packetIntervalTime = std::string("1.0");
}

Udp::~Udp()
{
}

size_t Udp::GetPort()
{
  return this->m_port;
}

void Udp::SetPort(const size_t &port)
{
  this->m_port = port;
}

void Udp::SetPacketSize(const size_t &packetSize)
{
  this->m_packetSize = packetSize;
}

size_t Udp::GetPacketSize()
{
  return this->m_packetSize;
}

void Udp::SetMaxPacketCount(const size_t &maxPacketCount)
{
  this->m_maxPacketCount = maxPacketCount;
}

size_t Udp::GetMaxPacketCount()
{
  return this->m_maxPacketCount;
}

void Udp::SetPacketIntervalTime(const std::string &packetIntervalTime)
{
  this->m_packetIntervalTime = packetIntervalTime;
}

std::string Udp::GetPacketIntervalTime()
{
  return this->m_packetIntervalTime;
}

std::vector<std::string> Udp::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/applications-module.h\"");

  return headers;
}

std::vector<std::string> Udp::GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice)
{
  std::vector<std::string> apps;

  std::string getReceiverNode = this->GetReceiverNode();
  std::string recNode;

  std::string getSenderNode = this->GetSenderNode();
  std::string sendNode;

  apps.push_back("uint16_t port_" + this->GetAppName() + " = " + utils::integerToString(this->m_port) + ";"); 

  apps.push_back("");
  apps.push_back("UdpServerHelper server_" + this->GetAppName() + " (port_" + this->GetAppName() + ");");
  apps.push_back("");
  
  if (this->GetReceiverNode().find("term_") == 0)
    recNode = "all_hub_0.Get(" + getReceiverNode.substr(getReceiverNode.find("_") + 1) + ")";
  else if (this->GetReceiverNode().find("station_") == 0)
    recNode = "all_ap_0.Get(" + getReceiverNode.substr(getReceiverNode.find("_") + 1) + ")";

  apps.push_back("ApplicationContainer apps_" + this->GetAppName() + " = server_" + this->GetAppName() + ".Install (" + recNode + ");");

  apps.push_back("apps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".0));");
  apps.push_back("apps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  apps.push_back("");
  apps.push_back("Time interPacketInterval_" + this->GetAppName() + " = Seconds (" + this->m_packetIntervalTime + ");");
  apps.push_back("");

  apps.push_back("UdpClientHelper client_" + this->GetAppName() + " (iface_" + netDeviceContainer + ".GetAddress(" + utils::integerToString(numberIntoNetDevice) + "), " + utils::integerToString(this->m_port) + ");");
  apps.push_back("client_" + this->GetAppName() + ".SetAttribute (\"MaxPackets\", UintegerValue (" + utils::integerToString(this->m_maxPacketCount) + "));");
  apps.push_back("client_" + this->GetAppName() + ".SetAttribute (\"Interval\", TimeValue (interPacketInterval_" + this->GetAppName() + "));");
  apps.push_back("client_" + this->GetAppName() + ".SetAttribute (\"PacketSize\", UintegerValue (" + utils::integerToString(this->m_packetSize) + "));");

  if (this->GetSenderNode().find("term_") == 0)
    sendNode = "all_hub_0.Get(" + getSenderNode.substr(getSenderNode.find("_") + 1) + ")";
  else if (this->GetSenderNode().find("station_") == 0)
    sendNode = "all_ap_0.Get(" + getSenderNode.substr(getSenderNode.find("_") + 1) + ")";

  apps.push_back("apps_" + this->GetAppName() + " = client_" + this->GetAppName() + ".Install (" + sendNode + ");");

  apps.push_back("apps_" + this->GetAppName() + ".Start (Seconds (" + this->GetStartTime() + ".1));");
  apps.push_back("apps_" + this->GetAppName() + ".Stop (Seconds (" + this->GetEndTime() + ".0));");

  return apps;
}
