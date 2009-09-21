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
 * \file emu.cpp
 * \brief Emu link subclass.
 * \author Pierre Weiss
 * \date 2009
 */

#include "emu.h"

Emu::Emu(const size_t &indice, const std::string &emuNode, const std::string &ifaceName) : Link(indice)
{
  this->Install(emuNode);
  this->SetEmuName(emuNode);
  this->SetIfaceName(ifaceName);
  this->SetLinkName(std::string("emu_" + this->GetIndice()));
  this->SetNdcName(std::string("ndc_" + this->GetLinkName()));
  this->SetAllNodeContainer(std::string("all_" + this->GetLinkName()));
}

Emu::~Emu()
{
}

std::vector<std::string> Emu::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/bridge-module.h\"");
  headers.push_back("#include \"ns3/emu-helper.h\"");

  return headers;
}

std::vector<std::string> Emu::GenerateLink()
{
  std::vector<std::string> generatedLink;
  generatedLink.push_back("EmuHelper " + this->GetLinkName() + ";");
  generatedLink.push_back(this->GetLinkName() + ".SetAttribute (\"DeviceName\", StringValue (emuDevice_" + this->GetLinkName() + "));");

  return generatedLink;
}

std::vector<std::string> Emu::GenerateNetDevice()
{
  std::vector<std::string> ndc;
  std::vector<std::string> allNodes = this->GroupAsNodeContainer();
  for(size_t i = 0; i < (size_t) allNodes.size(); i++)
  {
    ndc.push_back(allNodes.at(i));
  }
  ndc.push_back("NetDeviceContainer " + this->GetNdcName() + " = " + this->GetLinkName() + ".Install (" + this->GetAllNodeContainer() + ");");

  return ndc;
}

std::vector<std::string> Emu::GenerateVars()
{
  std::vector<std::string> vars;
  vars.push_back("std::string emuDevice_" + this->GetLinkName() + " = \"" + this->GetIfaceName() + "\";");
  return vars;
}

std::vector<std::string> Emu::GenerateCmdLine()
{
  std::vector<std::string> cmdLine;
  cmdLine.push_back("cmd.AddValue(\"deviceName_" + this->GetLinkName() + "\", \"device name\", emuDevice_" + this->GetLinkName() + ");");
  return cmdLine;
}

std::vector<std::string> Emu::GenerateTrace()
{
  std::vector<std::string> trace;

  if(this->GetTrace())
  {
    if(this->GetPromisc())
    {
      trace.push_back("EmuHelper::EnablePcap (\"Emu-" + this->GetLinkName() + "\",\"" + this->GetIfaceName() + "\", true);");
    }
    else
    {
      trace.push_back("EmuHelper::EnablePcap (\"Emu-" + this->GetLinkName() + "\",\"" + this->GetIfaceName() + "\", false);");
    }
  }

  return trace;
}

std::string Emu::GetEmuName()
{
  return this->m_emuNode;
}

void Emu::SetEmuName(const std::string &emuNode)
{
  this->m_emuNode = emuNode;
}

std::string Emu::GetIfaceName()
{
  return this->m_ifaceName;
}

void Emu::SetIfaceName(const std::string &ifaceName)
{
  this->m_ifaceName = ifaceName;
}
