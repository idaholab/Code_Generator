/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2016 Idaho National Laboratory
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
 * Author: Jeffrey M. Young <jeffrey.young@inl.gov>
 */

/**
 * \file flow.cpp
 * \brief Flow base class.
 * \author Jeffrey Young
 * \date 2016
 */

#include "flow.h"
#include "generator.h"

#include "utils.h"

Flow::Flow(const size_t &indice, const std::string &type, const std::string &flowName, const std::string &source, const std::string &destination, const std::string &expectedDelay, const std::string &expectedReliability)
{
  this->m_indice = indice;
  this->m_type = type;
//  this->m_modelName = modelName;
  this->m_flowName = flowName;
  this->m_source = source;
  this->m_destination = destination;
  this->m_expectedDelay = expectedDelay;
  this->m_expectedReliability = expectedReliability;
}

Flow::~Flow()
{
}

void Flow::SetFlowName(const std::string &flowName)
{
  this->m_flowName = flowName;
}

std::string Flow::GetFlowName()
{
  return this->m_flowName;
}

std::string Flow::GetIndice()
{
  return utils::integerToString(this->m_indice);
}

void Flow::SetIndice(const size_t &indice)
{
  this->m_indice = indice;
}

std::vector<std::string> Flow::GenerateHeader()
{
  std::vector<std::string> headers;
  headers.push_back("#include \"ns3/network-module.h\"");

  return headers; 
}

std::vector<std::string> Flow::GenerateFlowCpp()
{
  std::vector<std::string> flows;

  flows.push_back("");
  flows.push_back("query (\"UPDATE performancedata SET ExpectedReliability=" + this->m_expectedReliability + ", ExpectedDelay=" + this->m_expectedDelay + " WHERE ModelName='mymodelname'"  + " AND FromNode='" + this->m_source + "' AND ToNode='" + this->m_destination + "'\");");

  return flows; 
}

std::string Flow::GetFlowType()
{
  return this->m_type;
}
