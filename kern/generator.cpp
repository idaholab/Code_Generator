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
 * \file generator.cpp
 * \brief The main class of the generator.
 * \author Pierre Weiss
 * \date 2009
 */

#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <stdexcept>

#include "generator.h"
#include "node.h"
#include "hub.h"
#include "point-to-point.h"
#include "bridge.h"
#include "ap.h"
#include "ping.h"
#include "udp-echo.h"
#include "udp.h"
#include "udp-cosem.h"
#include "data-concentrator.h"
#include "meter-data-management.h"
#include "demand-response.h"
#include "tcp-large-transfer.h"
#include "tap.h"
#include "emu.h"
#include "utils.h"

using utils::split;

Generator::Generator(const std::string &simulationName)
{
  this->m_simulationName = simulationName;

  /* Node. */
  this->m_indiceNodePc = 0;
  this->m_indiceNodeRouter = 0;
  this->m_indiceNodeAp = 0;
  this->m_indiceNodeStation = 0;
  this->m_indiceNodeBridge = 0;
  this->m_indiceNodeTap = 0;
  this->m_indiceNodeEmu = 0;

  /* Link */
  this->m_indiceNetworkHardwareAp = 0;
  this->m_indiceNetworkHardwareEmu = 0;
  this->m_indiceNetworkHardwarePointToPoint = 0;
  this->m_indiceNetworkHardwareTap = 0;
  this->m_indiceNetworkHardwareHub = 0;
  this->m_indiceNetworkHardwareBridge = 0;

  /* Application */
  this->m_indiceApplicationPing = 0; 
  this->m_indiceApplicationTcpLargeTransfer = 0;
  this->m_indiceApplicationUdpEcho = 0;
  this->m_indiceApplicationUdp = 0;
  this->m_indiceApplicationUdpCosem = 0;
  this->m_indiceApplicationDataConcentrator = 0;
  this->m_indiceApplicationMeterDataManagement = 0;
  this->m_indiceApplicationDemandResponse = 0;

  // Flow
  this->m_indiceFlowWireless = 0;
  this->m_indiceFlowMultiHop = 0;
  this->m_indiceFlowWired = 0;


}

Generator::~Generator()
{
  /* Node */
  for(size_t i = 0; i < this->m_listNode.size(); i++)
  {
    delete this->m_listNode.at(i);
  }

  /* Link */
  for(size_t i = 0; i < this->m_listNetworkHardware.size(); i++)
  {
    delete this->m_listNetworkHardware.at(i);
  }

  /* Application */
  for(size_t i = 0; i < this->m_listApplication.size(); i++)
  {
    delete this->m_listApplication.at(i);
  }
  /* Flow */
  for(size_t i = 0; i < this->m_listFlow.size(); i++)
  {
    delete this->m_listFlow.at(i);
  }
}

void Generator::AddConfig(const std::string &config)
{
  bool isDuplicate = false;
  for(size_t i = 0; i < this->m_listConfiguration.size(); i++)
  {
    if(config == this->m_listConfiguration.at(i))
    {
      isDuplicate = true;
    }
  }
  if(!isDuplicate)
  {
    this->m_listConfiguration.push_back(config);
  }
}

//
// Part of Node.
//
void Generator::AddNode(const std::string &type, const std::string &nodeName) 
{
  size_t number = 1;
  Node *equi = NULL;

  // call to the right type constructor. 
  if(type == "Pc" || type == "Pc-group")
  {
    equi = new Node(this->m_indiceNodePc, type, nodeName, number);
    this->m_indiceNodePc += 1;
  } 
  else if(type == "Router")
  {
    equi = new Node(this->m_indiceNodeRouter, type, nodeName, number);
    this->m_indiceNodeRouter += 1;
  } 
  else if(type == "AccessPoint")
  {
    equi = new Node(this->m_indiceNodeAp, type, nodeName, number);
    this->m_indiceNodeAp += 1;
  } 
  else if(type == "Station")
  {
    equi = new Node(this->m_indiceNodeStation, type, nodeName, number);
    this->m_indiceNodeStation += 1;
  } 
  else if(type == "Bridge")
  {
    equi = new Node(this->m_indiceNodeBridge, type, nodeName, number);
    this->m_indiceNodeBridge += 1;
  } 
  else if(type == "Tap")
  {
    equi = new Node(this->m_indiceNodeTap, type, nodeName, number);
    this->m_indiceNodeTap += 1;
  } 
  else if(type == "Emu")
  {
    equi = new Node(this->m_indiceNodeEmu, type, nodeName, number);
    this->m_indiceNodeEmu += 1;
  }

  if(equi)
  {
    this->m_listNode.push_back(equi);
  }
  else
  {
    throw std::logic_error("Add Node failed! (" + type + ") unknow.");
  }
}

void Generator::AddNode(const std::string &type, const std::string &nodeName, const size_t number) 
{
  Node *equi = NULL;

  // call to the right type constructor. 
  if(type == "Pc" || type == "Pc-group")
  {
    equi = new Node(this->m_indiceNodePc, type, nodeName, number);
    this->m_indiceNodePc += 1;
  } 
  else if(type == "Router")
  {
    equi = new Node(this->m_indiceNodeRouter, type, nodeName, number);
    this->m_indiceNodeRouter += 1;
  } 
  else if(type == "AccessPoint")
  {
    equi = new Node(this->m_indiceNodeAp, type, nodeName, number);
    this->m_indiceNodeAp += 1;
  } 
  else if(type == "Station")
  {
    equi = new Node(this->m_indiceNodeStation, type, nodeName, number);
    this->m_indiceNodeStation += 1;
  } 
  else if(type == "Bridge")
  {
    equi = new Node(this->m_indiceNodeBridge, type, nodeName, number);
    this->m_indiceNodeBridge += 1;
  } 
  else if(type == "Tap")
  {
    equi = new Node(this->m_indiceNodeTap, type, nodeName, number);
    this->m_indiceNodeTap += 1;
  }
  else if(type == "Emu")
  {
    equi = new Node(this->m_indiceNodeEmu, type, nodeName, number);
    this->m_indiceNodeEmu += 1;
  }

  if(equi)
  {
    this->m_listNode.push_back(equi);
  }
  else
  {
    throw std::logic_error("Add Node failed! (" + type + ") unknow.");
  }
}


void Generator::RemoveNode(const std::string &name)
{
  size_t startNumber = this->m_listNode.size();
  for(size_t i = 0; i < this->m_listNode.size(); i++)
  {
    if(this->m_listNode.at(i)->GetNodeName() == name)
    {
      delete this->m_listNode[i];
      this->m_listNode.erase(this->m_listNode.begin() + i);
      break;
    }
  }
  size_t endNumber = this->m_listNode.size();
  if(startNumber == endNumber)
  {
    throw std::logic_error("Node remove failed! (" + name + ") not found.");
  }
}

void Generator::RemoveNode(const size_t index)
{
  if(this->m_listNode.size() < index)
  {
    throw std::out_of_range("Node remove failed ! (index not exists).");
    return;
  }
  
  delete this->m_listNode[index];
  this->m_listNode.erase(this->m_listNode.begin() + index);
}

Node* Generator::GetNode(const size_t index)
{
  if(this->m_listNode.size() < index)
  {
    throw std::out_of_range("Index does not exist.");
    return 0;
  }

  return this->m_listNode.at(index);
}

size_t Generator::GetNNodes() const
{
  return this->m_listNode.size();
}


//
// Part of Application.
//
void Generator::AddApplication(const std::string &type, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &port) 
{
  if(type == "UdpEcho")
  {
    UdpEcho *app = new UdpEcho(type, this->m_indiceApplicationUdpEcho, senderNode, receiverNode, startTime, endTime, port);
    this->m_indiceApplicationUdpEcho += 1;
    this->m_listApplication.push_back(app);
  }

  else if(type == "Udp")
  {
    Udp *app = new Udp(type, this->m_indiceApplicationUdp, senderNode, receiverNode, startTime, endTime, port);
    this->m_indiceApplicationUdp += 1;
    this->m_listApplication.push_back(app);
  }

  else if(type == "UdpCosem")
  {
    UdpCosem *app = new UdpCosem(type, this->m_indiceApplicationUdpCosem, senderNode, receiverNode, startTime, endTime, port);
    this->m_indiceApplicationUdpCosem += 1;
    this->m_listApplication.push_back(app);
  }

  else if(type == "TcpLargeTransfer")
  {
    TcpLargeTransfer *app = new TcpLargeTransfer(type, this->m_indiceApplicationTcpLargeTransfer, senderNode, receiverNode, startTime, endTime, port);
    this->m_indiceApplicationTcpLargeTransfer += 1;
    this->m_listApplication.push_back(app);
  }

  else
  {
    throw std::logic_error("Application add failed.");
  }

}

void Generator::AddApplication(const std::string &type, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime) 
{
  if(type == "Ping")
  {
    Ping *app = new Ping(type, this->m_indiceApplicationPing, senderNode, receiverNode, startTime, endTime);
    this->m_indiceApplicationPing += 1;
    this->m_listApplication.push_back(app);
  }

  else if(type == "DataConcentrator")
  {
    DataConcentrator *app = new DataConcentrator(type, this->m_indiceApplicationDataConcentrator, senderNode, receiverNode, startTime, endTime);
    this->m_indiceApplicationDataConcentrator += 1;
    this->m_listApplication.push_back(app);
  }

  else if(type == "DemandResponse")
  {
    DemandResponse *app = new DemandResponse(type, this->m_indiceApplicationDemandResponse, senderNode, receiverNode, startTime, endTime);
    this->m_indiceApplicationDemandResponse += 1;
    this->m_listApplication.push_back(app);
  }

  else
  {
    throw std::logic_error("Application add failed.");
  } 
}

void Generator::AddApplication(const std::string &type, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &interval, const size_t &readingTime) 
{
  if(type == "MeterDataManagement")
  {
    MeterDataManagement *app = new MeterDataManagement(type, this->m_indiceApplicationMeterDataManagement, senderNode, receiverNode, startTime, endTime, interval, readingTime);
    this->m_indiceApplicationMeterDataManagement += 1;
    this->m_listApplication.push_back(app);
  }

  else
  {
    throw std::logic_error("Application add failed.");
  } 
}

void Generator::RemoveApplication(const std::string &name)
{
  size_t startNumber = this->m_listApplication.size();
  for(size_t i = 0; i < this->m_listApplication.size(); i++)
  {
    if(this->m_listApplication.at(i)->GetAppName() == name)
    {
      delete this->m_listApplication[i];
      this->m_listApplication.erase(this->m_listApplication.begin() + i);
      break;
    }
  }
  size_t endNumber = this->m_listApplication.size();
  if(startNumber == endNumber)
  {
    throw std::logic_error("Application remove failed! (" + name + ") not found.");
  }
}

Application* Generator::GetApplication(const size_t index)
{
  if(this->m_listApplication.size() < index)
  {
    throw std::out_of_range("Index does not exist");
    return 0;
  }

  return this->m_listApplication.at(index);
}

size_t Generator::GetNApplications() const
{
  return this->m_listApplication.size();
}

//
// Part of Link.
//
void Generator::AddNetworkHardware(const std::string &type) 
{
  // call to the right type constructor. 
  if(type == "Hub")
  {
    Hub *link = new Hub(type, this->m_indiceNetworkHardwareHub);
    this->m_indiceNetworkHardwareHub += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else if(type == "PointToPoint")
  {
    PointToPoint *link = new PointToPoint(type, this->m_indiceNetworkHardwarePointToPoint);
    this->m_indiceNetworkHardwarePointToPoint += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else
  {
    throw std::logic_error("Add Link failed. ( " + type + ") not found.");
  }
}

void Generator::AddNetworkHardware(const std::string &type, const std::string &linkNode) 
{
  if(type == "Bridge")
  {
    Bridge *link = new Bridge(type, this->m_indiceNetworkHardwareBridge, linkNode);
    this->m_indiceNetworkHardwareBridge += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else if(type == "Ap")
  {
    Ap *link = new Ap(type, this->m_indiceNetworkHardwareAp, linkNode);
    this->m_indiceNetworkHardwareAp += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else
  {
    throw std::logic_error("Add Link failed. (" + type + ") not found.");
  }
}

void Generator::AddNetworkHardware(const std::string &type, const std::string &linkNode, const std::string &ifaceName)
{ 
  if(type == "Emu")
  {
    Emu *link = new Emu(type, this->m_indiceNetworkHardwareEmu, linkNode, ifaceName);
    this->m_indiceNetworkHardwareEmu += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else if(type == "Tap")
  {
    Tap *link = new Tap(type, this->m_indiceNetworkHardwareTap, linkNode, ifaceName);
    this->m_indiceNetworkHardwareTap += 1;
    this->m_listNetworkHardware.push_back(link);
  } 
  else
  {
    throw std::logic_error("Add Link failed. (" + type + ") not found.");
  }
}

void Generator::RemoveNetworkHardware(const std::string &name)
{
  size_t startNumber = this->m_listNetworkHardware.size();
  for(size_t i = 0; i < this->m_listNetworkHardware.size(); i++)
  {
    if(this->m_listNetworkHardware.at(i)->GetNetworkHardwareName() == name)
    {
      delete this->m_listNetworkHardware[i];
      this->m_listNetworkHardware.erase(this->m_listNetworkHardware.begin() + i);
      break;
    }
  }
  size_t endNumber = this->m_listNetworkHardware.size();
  if(startNumber == endNumber)
  {
    throw std::logic_error("Link remove failed! (" + name + ") not found.");
  }
}

void Generator::RemoveNetworkHardware(const size_t index)
{
  if(this->m_listNetworkHardware.size() < index)
  {
    throw std::out_of_range("Link remove failed! (index not exists).");
    return;
  }

  delete this->m_listNetworkHardware[index];
  this->m_listNetworkHardware.erase(this->m_listNetworkHardware.begin() + index);
}

NetworkHardware* Generator::GetNetworkHardware(const size_t index)
{
  if(this->m_listNetworkHardware.size() < index)
  {
    throw std::out_of_range("Index does not exist.");
    return 0;
  }
 
  return this->m_listNetworkHardware.at(index);
}

size_t Generator::GetNNetworkHardwares() const
{
  return this->m_listNetworkHardware.size();
}

//
// Part of Flow.
//
void Generator::AddFlow(const std::string &type, const std::string &flowName, const std::string &source, const std::string &destination, const std::string &expectedDelay, const std::string &expectedReliability) 
{
/// size_t number = 1;
  Flow *equi = NULL;

  // call to the right type constructor. 
  if(type == "WifiFlow")
  {
    equi = new Flow(this->m_indiceFlowWired, type, flowName, source, destination, expectedDelay, expectedReliability);
    this->m_indiceFlowWired += 1;
  } 
  else if(type == "MultiHopFlow")
  {
    equi = new Flow(this->m_indiceFlowMultiHop, type, flowName, source, destination, expectedDelay, expectedReliability);
    this->m_indiceFlowMultiHop += 1;
  } 
  else if(type == "WiredFlow")
  {
    equi = new Flow(this->m_indiceFlowWired, type, flowName, source, destination, expectedDelay, expectedReliability);
    this->m_indiceFlowWired += 1;
  } 

  if(equi)
  {
    this->m_listFlow.push_back(equi);
  }
  else
  {
    throw std::logic_error("Add Flow failed! (" + type + ") unknow.");
  }
}

void Generator::RemoveFlow(const std::string &name)
{
  size_t startNumber = this->m_listFlow.size();
  for(size_t i = 0; i < this->m_listFlow.size(); i++)
  {
    if(this->m_listFlow.at(i)->GetFlowName() == name)
    {
      delete this->m_listFlow[i];
      this->m_listFlow.erase(this->m_listFlow.begin() + i);
      break;
    }
  }
  size_t endNumber = this->m_listFlow.size();
  if(startNumber == endNumber)
  {
    throw std::logic_error("Flow remove failed! (" + name + ") not found.");
  }
}


//
// Part around the C++ code Generation.
// This part is looking about the code to write.
//

void Generator::GenerateCodeCpp(std::string fileName) 
{
  if(fileName != ""){
    this->m_cppFile.open(fileName.c_str());
  }

  this->WriteCpp("//////////////////////////////////////////////////////////////////////////////////////");
  this->WriteCpp("//");
  this->WriteCpp("// This source file was automatically generated using <" + this->m_simulationName + "> XML file.");
  this->WriteCpp("//");
  this->WriteCpp("//////////////////////////////////////////////////////////////////////////////////////");

this->WriteCpp("");

  //
  // Generate headers 
  //
  this->WriteCpp("#include \"ns3/core-module.h\"");
  this->WriteCpp("#include \"ns3/global-route-manager.h\"");
  this->WriteCpp("#include \"ns3/internet-module.h\"");
  this->WriteCpp("");
  this->WriteCpp("#include \"ns3/flow-monitor-helper.h\"");
  this->WriteCpp("#include \"ns3/ipv4-global-routing-helper.h\"");
  this->WriteCpp("#include \"ns3/ipv4-flow-classifier.h\"");
  this->WriteCpp("");
  this->WriteCpp("#include <map>");

  this->WriteCpp("");

  std::vector<std::string> allHeaders = GenerateHeader();
  for(size_t i = 0; i <  allHeaders.size(); i++)
  {
    this->WriteCpp("" + allHeaders.at(i));
  }

  this->WriteCpp("#include <sqlite3.h>");

  this->WriteCpp("using namespace ns3;");
  this->WriteCpp("using namespace std;");
  this->WriteCpp("");

//added logging statements
  std::string prefixName(fileName, 0, fileName.find('.'));
  this->WriteCpp("NS_LOG_COMPONENT_DEFINE (\"" + prefixName + "\");");

  this->WriteCpp("");

  this->WriteCpp("template <typename T>");
  this->WriteCpp("string NumberToString ( T Number )");
  this->WriteCpp("{");
  this->WriteCpp("	stringstream ss;");
  this->WriteCpp("	ss << Number;");
  this->WriteCpp("	return ss.str();");
  this->WriteCpp("}");
  this->WriteCpp("");

  this->WriteCpp("sqlite3 *database;");
  this->WriteCpp("sqlite3_stmt *statement;");
  this->WriteCpp("vector<vector<string> >  query(const char *query)");
  this->WriteCpp("{");
  this->WriteCpp("    vector<vector<string> > results;");
  this->WriteCpp("");
  this->WriteCpp("    if(sqlite3_prepare_v2(database, query, -1, &statement, 0) == SQLITE_OK)");
  this->WriteCpp("    {");
  this->WriteCpp("        int cols = sqlite3_column_count(statement);");
  this->WriteCpp("        int result = 0;");
  this->WriteCpp("        while(true)");
  this->WriteCpp("        {");
  this->WriteCpp("            result = sqlite3_step(statement);");
  this->WriteCpp("");
  this->WriteCpp("            if(result == SQLITE_ROW)");
  this->WriteCpp("            {");
  this->WriteCpp("                vector<string> values;");
  this->WriteCpp("                for(int col = 0; col < cols; col++)");
  this->WriteCpp("                {");
  this->WriteCpp("                    values.push_back((char*)sqlite3_column_text(statement, col));");
  this->WriteCpp("                }");
  this->WriteCpp("                results.push_back(values);");
  this->WriteCpp("            }");
  this->WriteCpp("            else");
  this->WriteCpp("            {");
  this->WriteCpp("                break;  ");
  this->WriteCpp("            }");
  this->WriteCpp("        }");
  this->WriteCpp("");
  this->WriteCpp("        sqlite3_finalize(statement);");
  this->WriteCpp("    }");
  this->WriteCpp("");
  this->WriteCpp("    string error = sqlite3_errmsg(database);");
  this->WriteCpp("    if(error != \"not an error\") std::cout << query << \" \" << error << std::endl;");
  this->WriteCpp("");
  this->WriteCpp("    return results; ");
  this->WriteCpp("}");

  this->WriteCpp("int main(int argc, char *argv[])");
  this->WriteCpp("{");

  //
  // Tap/Emu variables
  //
  std::vector<std::string> allVars = GenerateVarsCpp();
  for(size_t i = 0; i <  allVars.size(); i++)
  {
    this->WriteCpp("  " + allVars.at(i));
    this->WriteCpp("");
  }

  //
  // Generate Command Line 
  //
  this->WriteCpp("  CommandLine cmd;");

  std::vector<std::string> allCmdLine = GenerateCmdLineCpp();
  for(size_t i = 0; i <  allCmdLine.size(); i++)
  {
    this->WriteCpp("  " + allCmdLine.at(i));
  } 

  this->WriteCpp("  cmd.Parse (argc, argv);");
  this->WriteCpp("");

  this->WriteCpp("  std::string modelName = \"mymodelname\";");

//added logging statements
  this->WriteCpp("");
/// Added for cosem simulation only
  this->WriteCpp("/*");
     this->WriteCpp("LogComponentEnable (\"CosemApplicationLayerClient\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"CosemApplicationLayerServer\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"CosemApplicationsProcessClient\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"CosemApplicationsProcessServer\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"UdpCosemWrapperLayerClient\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"UdpCosemWrapperLayerServer\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"DataConcentratorApplication\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"DemandResponseApplication\", LOG_LEVEL_INFO);");
     this->WriteCpp("LogComponentEnable (\"MeterDataManagementApplication\", LOG_LEVEL_INFO);");
  this->WriteCpp("*/");
  this->WriteCpp("");

  this->WriteCpp("  Time::SetResolution (Time::NS);");
/***
  this->WriteCpp("  LogComponentEnable (\"UdpEchoClientApplication\", LOG_LEVEL_INFO);");
  this->WriteCpp("  LogComponentEnable (\"UdpEchoServerApplication\", LOG_LEVEL_INFO);");
***/
  this->WriteCpp("  ///LogComponentEnableAll (LOG_LEVEL_INFO);");
  this->WriteCpp("");

  //
  // Generate Optional configuration
  // 
  this->WriteCpp("");
  this->WriteCpp("  /* Configuration. */");
  std::vector<std::string> conf = GenerateConfigCpp();
  for(size_t i = 0; i <  conf.size(); i++)
  {
    this->WriteCpp("  " + conf.at(i));
  }

  //
  // Generate Nodes. 
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Build nodes. */");
  std::vector<std::string> nodeBuild = GenerateNodeCpp();
  for(size_t i = 0; i <  nodeBuild.size(); i++)
  {
    this->WriteCpp("  " + nodeBuild.at(i));
  }

  //
  // Generate Link.
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Build link. */");
  std::vector<std::string> linkBuild = GenerateNetworkHardwareCpp(); 
  for(size_t i = 0; i <  linkBuild.size(); i++)
  {
    this->WriteCpp("  " + linkBuild.at(i));
  }

  //
  // Generate link net device container.
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Build link net device container. */");

  std::vector<std::string> linkNdcBuild = GenerateNetDeviceCpp(); 
  for(size_t i = 0; i <  linkNdcBuild.size(); i++)
  {
    this->WriteCpp("  " + linkNdcBuild.at(i));
  }

  //
  // Generate Ip Stack. 
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Install the IP stack. */");
  std::vector<std::string> allStacks = GenerateIpStackCpp();
  for(size_t i = 0; i <  allStacks.size(); i++)
  {
    this->WriteCpp("  " + allStacks.at(i));
  }

  //
  // Generate IP assignation.
  // 
  this->WriteCpp("");
  this->WriteCpp("  /* IP assign. */");
  this->WriteCpp("  map<string, string> ipMap;");
  std::vector<std::string> allAssign = GenerateIpAssignCpp();
  for(size_t i = 0; i <  allAssign.size(); i++)
  {
    this->WriteCpp("  " + allAssign.at(i));
  } 

  //
  // Generate TapBridge if tap is used.
  //
  std::vector<std::string> allTapBridge = GenerateTapBridgeCpp();
  if(allTapBridge.size() > 0)
  {
    this->WriteCpp("");
    this->WriteCpp("  /* Tap Bridge. */");
  }
  for(size_t i = 0; i <  allTapBridge.size(); i++)
  {
    this->WriteCpp("  " + allTapBridge.at(i));
  } 

  //
  // Generate Route.
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Generate Route. */");
  std::vector<std::string> allRoutes = GenerateRouteCpp();
  for(size_t i = 0; i <  allRoutes.size(); i++)
  {
    this->WriteCpp("  " + allRoutes.at(i));
  } 

  //
  // Generate Application.
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Generate Application. */");
  std::vector<std::string> allApps = GenerateApplicationCpp();
  for(size_t i = 0; i <  allApps.size(); i++)
  {
    this->WriteCpp("  " + allApps.at(i));
  } 

  //
  // Others
  //
  this->WriteCpp("");
  this->WriteCpp("  /* Simulation. */");
  this->WriteCpp("");

  this->WriteCpp("  /* Pcap output. */");
  std::vector<std::string> allTrace = GenerateTraceCpp();
  for(size_t i = 0; i <  allTrace.size(); i++)
  {
    this->WriteCpp("  " + allTrace.at(i));
  } 

  this->WriteCpp("");
  this->WriteCpp("  /* Flow Monitor */");

  this->WriteCpp("  FlowMonitorHelper flowmonHelper;");
///  this->WriteCpp("  Ptr<FlowMonitor> monitor = flowmonHelper.InstallAll();");
  this->WriteCpp("  Ptr<FlowMonitor> monitor;");

  std::vector<std::string> allFlowmon = GenerateFlowmonitor();
  for(size_t i = 0; i <  allFlowmon.size(); i++)
  {
    this->WriteCpp("  " + allFlowmon.at(i));
  } 

  /* Set stop time. */
  size_t stopTime = 0;/* default stop time. */
  for(size_t i = 0; i <  this->m_listApplication.size(); i++)
  {
    if( (this->m_listApplication.at(i))->GetEndTimeNumber() > stopTime)
    {
      stopTime = (this->m_listApplication.at(i))->GetEndTimeNumber();
    }
  }
  stopTime += 50;

  this->WriteCpp("");

  this->WriteCpp("  /* Stop the simulation after x seconds. */");
  this->WriteCpp("  uint32_t stopTime = "+ utils::integerToString(stopTime) +";"); 
  this->WriteCpp("  Simulator::Stop (Seconds (stopTime));");

  this->WriteCpp("");
  this->WriteCpp("  /* Start and clean simulation. */");
  this->WriteCpp("  Simulator::Run ();");
  this->WriteCpp("  flowmonHelper.SerializeToXmlFile (\"" + prefixName + ".flowmonitor\", true, true);");

/// Flowmonitor and database
  std::vector<std::string> allMetrics = GenerateMetrics();
  for(size_t i = 0; i <  allMetrics.size(); i++)
  {
    this->WriteCpp("  " + allMetrics.at(i));
  } 

  this->WriteCpp("");
  this->WriteCpp("  Simulator::Destroy ();");

  this->WriteCpp("}"); 
  
  if(fileName != ""){
    this->m_cppFile.close();
  }
}

std::vector<std::string> Generator::GenerateHeader() 
{
  std::vector<std::string> allHeaders;
  // get all headers.
  /* from m_listNode. */
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNode.at(i))->GenerateHeader();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allHeaders.push_back(trans.at(j));
    }
  }
  /* from m_listLink */
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateHeader();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allHeaders.push_back(trans.at(j));
    }
  }
  /* from m_listApplication */
  for(size_t i = 0; i <  this->m_listApplication.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listApplication.at(i))->GenerateHeader();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allHeaders.push_back(trans.at(j));
    }
  }

  /* check for duplicate */
  std::vector<std::string> headersWithoutDuplicateElem;
  bool isDuplicate = false;
  /* iterate all headers string */
  for(size_t i = 0; i <  allHeaders.size(); i++)
  {
    isDuplicate = false;
    /* iterate the vector whith no duplicate */
    for(size_t j = 0; j <  headersWithoutDuplicateElem.size(); j++)
    {
      /* check if the string into the allHeaders vector is also in the vector without duplicate */
      if( allHeaders.at(i) == headersWithoutDuplicateElem.at(j))
      {
        /* it's an duplicated elem. */
        isDuplicate = true;
        break;
      }	
    }
    /* add the string from allHeaders if no duplicate have been detected. */
    if(!isDuplicate)
    {
      headersWithoutDuplicateElem.push_back(allHeaders.at(i));
    }
  }
  return headersWithoutDuplicateElem;
}

std::vector<std::string> Generator::GenerateVarsCpp()
{
  std::vector<std::string> allVars;
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateVarsCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allVars.push_back(trans.at(j));
    }
  }

  /* add nsc var if used. */
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    if( (this->m_listNode.at(i))->GetNsc() != "")
    {
      allVars.push_back("std::string nscStack = \"" + (this->m_listNode.at(i))->GetNsc() + "\";");
    }
  }
  return allVars;
}

std::vector<std::string> Generator::GenerateCmdLineCpp() 
{
  std::vector<std::string> allCmdLine;
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateCmdLineCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allCmdLine.push_back(trans.at(j));
    }
  }
  return allCmdLine;
}

std::vector<std::string> Generator::GenerateConfigCpp() 
{
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    if( ((this->m_listNode.at(i))->GetNodeName()).find("tap_") == 0)
    {
      this->AddConfig("GlobalValue::Bind (\"SimulatorImplementationType\", StringValue (\"ns3::RealtimeSimulatorImpl\"));");
      this->AddConfig("GlobalValue::Bind (\"ChecksumEnabled\", BooleanValue (true));");
    }
  }

  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  { 
    if( ((this->m_listNetworkHardware.at(i))->GetNetworkHardwareName()).find("emu_") == 0 )
    {
      this->AddConfig("GlobalValue::Bind (\"SimulatorImplementationType\", StringValue (\"ns3::RealtimeSimulatorImpl\"));");
      this->AddConfig("GlobalValue::Bind (\"ChecksumEnabled\", BooleanValue (true));");
    }
  } 

  std::vector<std::string> allConf;
  for(size_t i = 0; i <  this->m_listConfiguration.size(); i++)
  {
    allConf.push_back(this->m_listConfiguration.at(i));
  }

  return allConf;
}

std::vector<std::string> Generator::GenerateNodeCpp() 
{
  std::vector<std::string> allNodes;
  /* get all the node code. */
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNode.at(i))->GenerateNodeCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allNodes.push_back(trans.at(j));
    }
  }
  return allNodes;
}

std::vector<std::string> Generator::GenerateNetworkHardwareCpp() 
{
  std::vector<std::string> allLink;
  /* get all the link build code. */
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateNetworkHardwareCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allLink.push_back(trans.at(j));
    }
  }
  return allLink;
}
std::vector<std::string> Generator::GenerateNetDeviceCpp() 
{
  std::vector<std::string> allNdc;
  /* get all the link build code. */
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateNetDeviceCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allNdc.push_back(trans.at(j));
    }
  }
  return allNdc;
}

std::vector<std::string> Generator::GenerateIpStackCpp() 
{
  std::vector<std::string> allStack;

  /* construct node without bridge Node. */
  allStack.push_back("InternetStackHelper internetStackH;");

  std::string nodeName = "";
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    nodeName = (this->m_listNode.at(i))->GetNodeName();
    // if it is not a bridge you can add it. /
    if(nodeName.find("bridge_") != 0)
    {
      std::vector<std::string> trans = (this->m_listNode.at(i)->GenerateIpStackCpp());
      for(size_t j = 0; j <  trans.size(); j++)
      {
        allStack.push_back(trans.at(j));
      }
    }
  }

  return allStack;
}

std::vector<std::string> Generator::GenerateIpAssignCpp() 
{
  std::vector<std::string> ipAssign;
  ipAssign.push_back("std::ostringstream addrOss;");
  ipAssign.push_back("Ipv4AddressHelper ipv4;");

  size_t ipRange = 0;
  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    ipAssign.push_back("ipv4.SetBase (\"10.0." + utils::integerToString(ipRange) + ".0\", \"255.255.255.0\");");
    ipAssign.push_back("Ipv4InterfaceContainer iface_" + this->m_listNetworkHardware.at(i)->GetNdcName() + " = ipv4.Assign (" + this->m_listNetworkHardware.at(i)->GetNdcName() + ");");

    if (this->m_listNetworkHardware.at(i)->GetNdcName().find("ndc_ap_") == 0)
    {
        ipAssign.push_back("iface_" + this->m_listNetworkHardware.at(i)->GetNdcName() + ".GetAddress(0).Print (addrOss);");
        ipAssign.push_back("ipMap [addrOss.str()] = Names::FindName (ap_" + this->m_listNetworkHardware.at(i)->GetIndice() + ".Get (0));");
        ipAssign.push_back("addrOss.str(\"\");");
        ipAssign.push_back("addrOss.clear();");
        ipAssign.push_back("///ipv4.Assign (ndc_sta_" + this->m_listNetworkHardware.at(i)->GetIndice() + ");");
        ipAssign.push_back("Ipv4InterfaceContainer iface_ndc_sta_" + this->m_listNetworkHardware.at(i)->GetIndice() + ";");

        ipAssign.push_back("for (uint32_t i = 0; i < " + this->m_listNetworkHardware.at(i)->GetAllNodeContainer() + ".GetN (); ++i)");
        ipAssign.push_back("  {");
        ipAssign.push_back("  iface_ndc_sta_" + this->m_listNetworkHardware.at(i)->GetIndice() + ".Add (ipv4.Assign (ndc_sta_" + this->m_listNetworkHardware.at(i)->GetIndice() + ".Get (i)));");
        ipAssign.push_back("  iface_ndc_sta_" + this->m_listNetworkHardware.at(i)->GetIndice() + ".GetAddress(i).Print (addrOss);");
        ipAssign.push_back("  ipMap [addrOss.str()] = Names::FindName (" + this->m_listNetworkHardware.at(i)->GetAllNodeContainer() + ".Get (i));");
        ipAssign.push_back("  addrOss.str(\"\");");
        ipAssign.push_back("  addrOss.clear();");
        ipAssign.push_back("  }");
    }
    else
    {
        ipAssign.push_back("for (uint32_t i = 0; i < " + this->m_listNetworkHardware.at(i)->GetAllNodeContainer() + ".GetN (); ++i)");
        ipAssign.push_back("  {");
        ipAssign.push_back("  iface_" + this->m_listNetworkHardware.at(i)->GetNdcName() + ".GetAddress(i).Print (addrOss);");
        ipAssign.push_back("  ipMap [addrOss.str()] = Names::FindName (" + this->m_listNetworkHardware.at(i)->GetAllNodeContainer() + ".Get (i));");
        ipAssign.push_back("  addrOss.str(\"\");");
        ipAssign.push_back("  addrOss.clear();");
        ipAssign.push_back("  }");
    }

std::cerr << "ndc name = " << this->m_listNetworkHardware.at(i)->GetNdcName() << std::endl;
std::cerr << "Node container name = " << this->m_listNetworkHardware.at(i)->GetAllNodeContainer() << std::endl;

    ipRange += 1;
  } 


  return ipAssign;
}

std::vector<std::string> Generator::GenerateRouteCpp() 
{
  std::vector<std::string> allRoutes;
 
  allRoutes.push_back("Ipv4GlobalRoutingHelper::PopulateRoutingTables ();");

  return allRoutes;
}

std::vector<std::string> Generator::GenerateApplicationCpp() 
{
  size_t nodeNumber = 0;
  std::string ndcName = "";
  size_t linkNumber = 0;
  std::vector<std::string> allApps;
  /* get all the ip assign code. */
  for(size_t i = 0; i <  this->m_listApplication.size(); i++)
  {
    /* get NetDeviceContainer and number from the receiver. */
    std::string receiverName = this->m_listApplication.at(i)->GetReceiverNode();
    nodeNumber = 0;
    ndcName = "";
    linkNumber = 0;

    /* if the receiver is in NodeContainer */
    if(receiverName.find("NodeContainer(") == 0)
    {
      std::string oldReceiverName = receiverName;
      
      std::vector<std::string> tab_name;
      split(tab_name, receiverName, '(');
      
      std::string str_get = tab_name.at(1);
      std::vector<std::string> tab_name2;
      split(tab_name2, str_get, '.');
      
      receiverName = tab_name2.at(0);
      for(size_t x = 0;  x < this->m_listNetworkHardware.size(); x++)
      {
        nodeNumber = 0;
        for(size_t y = 0; y < this->m_listNetworkHardware.at(x)->GetInstalledNodes().size(); y++)
        {
          if(this->m_listNetworkHardware.at(x)->GetInstalledNodes().at(y) == receiverName || this->m_listNetworkHardware.at(x)->GetInstalledNodes().at(y) == oldReceiverName)
          {
            ndcName = (this->m_listNetworkHardware.at(x))->GetNdcName();
            linkNumber = x;
            break;
          }
          if(ndcName != "")
          {
            break;
          }
        }
      }
      std::vector<std::string> linksNode = this->m_listNetworkHardware.at(linkNumber)->GetInstalledNodes();
      for(size_t j = 0; j < linksNode.size(); j++)
      {
        if(linksNode.at(j) == oldReceiverName)
        {
          nodeNumber = j;
          break;
        }
      }
    }
    else
    {
      for(size_t j = 0; j <  this->m_listNetworkHardware.size(); j++)
      {
        nodeNumber = 0;
        linkNumber = 0;
        std::vector<std::string> nodes = (this->m_listNetworkHardware.at(j))->GetInstalledNodes();
        for(size_t k = 0; k <  nodes.size(); k++)
        {
          if( nodes.at(k) == receiverName)
          {
            ndcName = (this->m_listNetworkHardware.at(j))->GetNdcName();
            break;
          }
          else
          {
            for(size_t l = 0; l <  this->m_listNode.size(); l++)
            {
              if(this->m_listNode.at(l)->GetNodeName() == nodes.at(k))
              {
                nodeNumber += this->m_listNode.at(l)->GetMachinesNumber();
                break;
              }
            }
          }
        }
        if(ndcName != "")
        {
          break;
        }
      }
    }
    /* get the application code with param. */
    std::vector<std::string> trans = (this->m_listApplication.at(i)->GenerateApplicationCpp(ndcName, nodeNumber));
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allApps.push_back(trans.at(j));
    }
  }

  return allApps;
}

std::vector<std::string> Generator::GenerateTapBridgeCpp()
{
  std::vector<std::string> allTapBridge;

  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateTapBridgeCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allTapBridge.push_back(trans.at(j));
    }
  }

  return allTapBridge;
}

std::vector<std::string> Generator::GenerateTraceCpp()
{
  std::vector<std::string> allTrace;

  for(size_t i = 0; i <  this->m_listNetworkHardware.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listNetworkHardware.at(i))->GenerateTraceCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allTrace.push_back(trans.at(j));
    }
  }

  return allTrace;
}

std::vector<std::string> Generator::GenerateFlowmonitor() 
{
  std::vector<std::string> allFlowmon;

  std::string nodeName = "";
  for(size_t i = 0; i <  this->m_listNode.size(); i++)
  {
    nodeName = (this->m_listNode.at(i))->GetNodeName();
      std::vector<std::string> trans = (this->m_listNode.at(i)->GenerateFlowmonitor());
      for(size_t j = 0; j <  trans.size(); j++)
      {
        allFlowmon.push_back(trans.at(j));
      }
  }

  return allFlowmon;
}

std::vector<std::string> Generator::GenerateFlowCpp() 
{
  std::vector<std::string> allFlows;
  /* get all the flow code. */
  for(size_t i = 0; i <  this->m_listFlow.size(); i++)
  {
    std::vector<std::string> trans = (this->m_listFlow.at(i))->GenerateFlowCpp();
    for(size_t j = 0; j <  trans.size(); j++)
    {
      allFlows.push_back(trans.at(j));
    }
  }
  return allFlows;
}

std::vector<std::string> Generator::GenerateMetrics() 
{
  std::vector<std::string> allMetrics;
 
///  allRoutes.push_back("Ipv4GlobalRoutingHelper::PopulateRoutingTables ();");

  allMetrics.push_back("  monitor->CheckForLostPackets ();");
  allMetrics.push_back("");

  allMetrics.push_back("	// open the database file");
  allMetrics.push_back("	if(sqlite3_open(\"pgcpmtDatabaseTest.sqlite\", &database) == SQLITE_OK)");
  allMetrics.push_back("        std::cout << \"database opened\" << std::endl;");
  allMetrics.push_back("    else");
  allMetrics.push_back("        std::cout << \"open database failed\" << std::endl;");
  allMetrics.push_back("    string mynone = NumberToString(0);");
  allMetrics.push_back("	// create node name and ip address table with data");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists nodeNameIpAdrs (ModelName VARCHAR (20), NodeName VARCHAR (20), IpAdrs VARCHAR (20));\");");
  allMetrics.push_back("	// create flow header table with data");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists flowHeader (ModelName VARCHAR (20), FlowId STRING, sourceNodeName VARCHAR (20), DestNodeName VARCHAR (20), Protocol VARCHAR (20), SourcePort STRING, DestPort STRING);\");");
  allMetrics.push_back("	// create the flow data table");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists flowData (ModelName VARCHAR (20), FlowId STRING, TxBitrate STRING, RxBitrate STRING, MeanDelay STRING, PacketLossRatio STRING);\");");
  allMetrics.push_back("	// create the packet data table");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists packetData (ModelName VARCHAR (20), FlowId STRING, FirstTxPacket STRING, FirstRxPacket STRING, LastTxPacket STRING, LastRxPacket STRING, DelaySum STRING, JitterSum STRING, LastDelay STRING, TxBytes STRING, RxBytes STRING, TxPackets STRING, RxPackets STRING, LostPackets STRING, TimesForwarded STRING, Throughput STRING);\");");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists performanceData (ModelName VARCHAR (20), FlowId STRING, Status VARCHAR (20), FromNode VARCHAR (20), ToNode VARCHAR (20), ExpectedReliability STRING, ExpectedDelay STRING, ActualReliability STRING, ActualDelay STRING, TxPackets STRING, RxPackets STRING, Throughput STRING, Cost STRING);\");");
  allMetrics.push_back("");
  allMetrics.push_back("	// database info");
  allMetrics.push_back("	double txBitrate = 0;");
  allMetrics.push_back("	double rxBitrate = 0;");
  allMetrics.push_back("	double meanDelay = 0;");
  allMetrics.push_back("	uint32_t packetLossRatio = 0;");
  allMetrics.push_back("");
  allMetrics.push_back("	double firstTxPacket = 0;");
  allMetrics.push_back("	double firstRxPacket = 0;");
  allMetrics.push_back("	double lastTxPacket = 0;");
  allMetrics.push_back("	double lastRxPacket = 0;");
  allMetrics.push_back("	double delaySum = 0;");
  allMetrics.push_back("	double jitterSum = 0;");
  allMetrics.push_back("	double lastDelay = 0;");
  allMetrics.push_back("	uint32_t txBytes = 0;");
  allMetrics.push_back("	uint32_t rxBytes = 0;");
  allMetrics.push_back("	uint32_t txPackets = 0;");
  allMetrics.push_back("	uint32_t rxPackets = 0;");
  allMetrics.push_back("	uint32_t lostPackets = 0;");
  allMetrics.push_back("	uint32_t timeForwarded = 0;");
  allMetrics.push_back("	double throughput = 0;");
  allMetrics.push_back("");

  allMetrics.push_back("  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmonHelper.GetClassifier ());");
  allMetrics.push_back("  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();");
  allMetrics.push_back("  uint32_t txPacketsum = 0;");
  allMetrics.push_back("  uint32_t rxPacketsum = 0;");
  allMetrics.push_back("  uint32_t DropPacketsum = 0;");
  allMetrics.push_back("  uint32_t LostPacketsum = 0;");
  allMetrics.push_back("  double Delaysum = 0;");
  allMetrics.push_back("  std::string protocolName;");
  allMetrics.push_back("  const char *DroppedNames[] = { \"No Route\", \"TTL Expire\", \"Bad Checksum\", \"Queue\", \"Interface Down\", \"Route error\", \"Fragment timeout\", \"Unknown\" };");

  allMetrics.push_back("  cout << \" \" << endl;");
  allMetrics.push_back("  cout << \" \" << endl;");
  allMetrics.push_back("  cout << \"IP Address		Node Name\" << endl;");
  allMetrics.push_back("  cout << \"----------		---------\" << endl;");
  allMetrics.push_back("  for (map<string, string>::iterator it = ipMap.begin(); it!=ipMap.end(); ++it)");
  allMetrics.push_back("{");
  allMetrics.push_back("    cout << it->first << \"		\" << it->second << endl;");
  allMetrics.push_back("	string myNodeName = it->second;");
  allMetrics.push_back("	string myIpAdrs = it->first;");
  allMetrics.push_back("	string flowDataQuery (\"INSERT INTO nodeNameIpAdrs VALUES(\"\"'\" + modelName + \"'\" + \", \" + \"'\" + myNodeName + \"'\" + \",\" + \"'\" + myIpAdrs + \"'\" + \");\"  );");
  allMetrics.push_back("	query( flowDataQuery.c_str() );");
  allMetrics.push_back("}");

  allMetrics.push_back("");
  allMetrics.push_back("  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)");
  allMetrics.push_back("  {");
  allMetrics.push_back("    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);");
  allMetrics.push_back("    txPacketsum += i->second.txPackets;");
  allMetrics.push_back("    rxPacketsum += i->second.rxPackets;");
  allMetrics.push_back("    LostPacketsum += i->second.lostPackets;");
  allMetrics.push_back("    DropPacketsum += i->second.packetsDropped.size();");
  allMetrics.push_back("    Delaysum += i->second.delaySum.GetSeconds();");
  allMetrics.push_back("/*    cout << \">\" << endl;");
  allMetrics.push_back("    cout << \">\" << endl;");
  allMetrics.push_back("    cout << \">\" << endl;");
  allMetrics.push_back("*/");
  allMetrics.push_back("    if (t.protocol == 17)");
  allMetrics.push_back("            protocolName = \"UDP\";");
  allMetrics.push_back("    else if (t.protocol == 6)");
  allMetrics.push_back("              protocolName = \"TCP\";");
  allMetrics.push_back("            else protocolName = \"UNK\";");
  allMetrics.push_back("            ");
  allMetrics.push_back("    cout << \"Flow ID: \" << i->first << endl;");
  allMetrics.push_back("    cout << \"======\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    std::ostringstream srcAddrOss;");
  allMetrics.push_back("    std::ostringstream dstAddrOss;");
  allMetrics.push_back("    t.sourceAddress.Print (srcAddrOss);");
  allMetrics.push_back("    t.destinationAddress.Print (dstAddrOss);");
  allMetrics.push_back("");
  allMetrics.push_back("    cout << protocolName << \" \" << ipMap[srcAddrOss.str()] << \"/\" << srcAddrOss.str() << \"/\" << t.sourcePort << \" ----> \" << ipMap[dstAddrOss.str()] << \"/\" << dstAddrOss.str() << \"/\" << t.destinationPort << endl;");
  allMetrics.push_back("    cout << \" \" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if ((i->second.timeLastTxPacket - i->second.timeFirstTxPacket) == 0)");
  allMetrics.push_back("      cout << \"Tx bitrate: 0 kbps\" << endl;");
  allMetrics.push_back("    else");
  allMetrics.push_back("      cout << \"Tx bitrate:\" << (8.0 * i->second.txBytes * 1e-3 * 1e9) / (i->second.timeLastTxPacket - i->second.timeFirstTxPacket) << \" kbps\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if ((i->second.timeLastRxPacket - i->second.timeFirstRxPacket) == 0)");
  allMetrics.push_back("      cout << \"Rx bitrate: 0 kbps\" << endl;");
  allMetrics.push_back("    else");
  allMetrics.push_back("      cout << \"Rx bitrate:\" << (8.0 * i->second.rxBytes * 1e-3 * 1e9) / (i->second.timeLastRxPacket - i->second.timeFirstRxPacket) << \" kbps\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if (i->second.rxPackets == 0)");
  allMetrics.push_back("      cout << \"Mean delay: 0 ms\" << endl;");
  allMetrics.push_back("    else cout << \"Mean delay:\" << (1000 * i->second.delaySum.GetSeconds()) / (i->second.rxPackets) << \" ms\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if ((i->second.rxPackets + i->second.lostPackets) == 0)");
  allMetrics.push_back("      cout << \"Packet Loss ratio: 0%\" << endl;");
  allMetrics.push_back("    else cout << \"Packet Loss ratio: \" << ((i->second.lostPackets * 1.0) / (i->second.rxPackets + i->second.lostPackets) * 1.0) * 100 << \"%\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    cout << \" \" << endl;");
  allMetrics.push_back("    cout << \"First Tx Packet: \" << i->second.timeFirstTxPacket.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"First Rx Packet: \" << i->second.timeFirstRxPacket.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Last Tx Packet: \" << i->second.timeLastTxPacket.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Last Rx Packet: \" << i->second.timeLastRxPacket.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Delay Sum: \" << i->second.delaySum.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Jitter Sum: \" << i->second.jitterSum.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Last Delay: \" << i->second.lastDelay.GetSeconds() << \" secs.\" << endl;");
  allMetrics.push_back("    cout << \"Tx Bytes: \" << i->second.txBytes << endl;");
  allMetrics.push_back("    cout << \"Rx Bytes: \" << i->second.rxBytes << endl;");
  allMetrics.push_back("    cout << \"Tx Packets: \" << i->second.txPackets << endl;");
  allMetrics.push_back("    cout << \"Rx Packets: \" << i->second.rxPackets << endl;");
  allMetrics.push_back("    cout << \"Lost Packets: \" << i->second.lostPackets << endl;");
  allMetrics.push_back("    cout << \"Times Forwarded: \" << i->second.timesForwarded << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if ((i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) == 0)");
  allMetrics.push_back("      cout << \"Throughput: 0 Kbps\" << endl;");
  allMetrics.push_back("    else cout << \"Throughput: \" << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024  << \" Kbps\" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    cout << \" \" << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("    if (!i->second.packetsDropped.empty())");
  allMetrics.push_back("      {");
  allMetrics.push_back("      cout << \"Packets Dropped:\" << endl;");
  allMetrics.push_back("      cout << \" \" << endl;");
  allMetrics.push_back("      }");
  allMetrics.push_back("");
  allMetrics.push_back("    for (uint32_t reasonCode = 0; reasonCode < i->second.packetsDropped.size (); reasonCode++)");
  allMetrics.push_back("    {");
  allMetrics.push_back("      cout << DroppedNames[reasonCode] << \":\" << i->second.packetsDropped[reasonCode] << endl;");
  allMetrics.push_back("    }");

  allMetrics.push_back("");
  allMetrics.push_back("    // insert flow header data into database");
  allMetrics.push_back("	string myFlowIdNumber = NumberToString(i->first);");
  allMetrics.push_back("	string mySourceIp = srcAddrOss.str();");
  allMetrics.push_back("	string mySourceName = ipMap[srcAddrOss.str()];");
  allMetrics.push_back("	string myDestName = ipMap[dstAddrOss.str()];");
  allMetrics.push_back("	string myDestIp = dstAddrOss.str();");
  allMetrics.push_back("	string myProtocol = NumberToString(Delaysum / txPacketsum);");
  allMetrics.push_back("	string mySourcePort = NumberToString(t.sourcePort);");
  allMetrics.push_back("	string myDestPort = NumberToString(t.destinationPort);");
  allMetrics.push_back("	string flowHeaderQuery (\"INSERT INTO flowHeader VALUES(\"\"'\" + modelName + \"'\" + \", \" + myFlowIdNumber + \",\" + \"'\" + mySourceName + \"'\" + \",\" + \"'\" + myDestName + \"'\" + \",\" + \"'\" + protocolName + \"'\"  + \",\" + mySourcePort + \",\" + myDestPort + \");\"  );");
  allMetrics.push_back("	query( flowHeaderQuery.c_str() );");
  allMetrics.push_back("");
  allMetrics.push_back("	// insert flow data into database");
  allMetrics.push_back("	string mytxBitrate = NumberToString(txBitrate);");
  allMetrics.push_back("	string myrxBitrate = NumberToString(rxBitrate);");
  allMetrics.push_back("	string mymeanDelay = NumberToString(meanDelay);");
  allMetrics.push_back("	string mypacketLossRatio = NumberToString(packetLossRatio);");
  allMetrics.push_back("	string flowDataQuery (\"INSERT INTO flowData VALUES(\"\"'\" + modelName + \"'\" + \", \" + myFlowIdNumber + \",\" + mytxBitrate + \",\" + myrxBitrate + \",\" + mymeanDelay + \",\" + mypacketLossRatio + \");\"  );");
  allMetrics.push_back("	query( flowDataQuery.c_str() );");
  allMetrics.push_back("");
  allMetrics.push_back("    // packet data into database");
  allMetrics.push_back("	firstTxPacket = i->second.timeFirstTxPacket.GetSeconds();");
  allMetrics.push_back("	firstRxPacket = i->second.timeFirstRxPacket.GetSeconds();	");
  allMetrics.push_back("	lastTxPacket = i->second.timeLastTxPacket.GetSeconds();");
  allMetrics.push_back("	lastRxPacket = i->second.timeLastRxPacket.GetSeconds();	");
  allMetrics.push_back("	delaySum = i->second.delaySum.GetSeconds();");
  allMetrics.push_back("	jitterSum = i->second.jitterSum.GetSeconds();	");
  allMetrics.push_back("	lastDelay = i->second.lastDelay.GetSeconds();	");
  allMetrics.push_back("	txBytes = i->second.txBytes;");
  allMetrics.push_back("	rxBytes = i->second.rxBytes;");
  allMetrics.push_back("	txPackets = i->second.txPackets;");
  allMetrics.push_back("	rxPackets = i->second.rxPackets;");
  allMetrics.push_back("	timeForwarded = i->second.timesForwarded;");
  allMetrics.push_back("	throughput = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024;	");
  allMetrics.push_back("");
  allMetrics.push_back("	string myfirstTxPacket = NumberToString(firstTxPacket);");
  allMetrics.push_back("	string myfirstRxPacket = NumberToString(firstRxPacket);");
  allMetrics.push_back("	string mylastTxPacket = NumberToString(lastTxPacket);");
  allMetrics.push_back("	string mylastRxPacket = NumberToString(lastRxPacket);");
  allMetrics.push_back("	string mydelaySum = NumberToString(delaySum);");
  allMetrics.push_back("	string myjitterSum = NumberToString(jitterSum);");
  allMetrics.push_back("	string mylastDelay = NumberToString(lastDelay);");
  allMetrics.push_back("	string mytxBytes = NumberToString(txBytes);");
  allMetrics.push_back("	string myrxBytes = NumberToString(rxBytes);");
  allMetrics.push_back("	string mytxPackets = NumberToString(txPackets);");
  allMetrics.push_back("	string myrxPackets = NumberToString(rxPackets);");
  allMetrics.push_back("	string mylostPackets = NumberToString(lostPackets);");
  allMetrics.push_back("	string mytimeForwarded = NumberToString(timeForwarded);");
  allMetrics.push_back("	string mythroughput = NumberToString(throughput);");
  allMetrics.push_back("	string packetDataQuery (\"INSERT INTO packetData VALUES(\"\"'\" + modelName + \"'\" + \", \" + myFlowIdNumber + \",\" + myfirstTxPacket + \",\" + myfirstRxPacket + \",\" + mylastTxPacket + \",\" + mylastRxPacket + \",\" + mydelaySum + \",\" + myjitterSum + \",\" + mylastDelay + \",\" + mytxBytes + \",\" + myrxBytes + \",\" + mytxPackets + \",\" + myrxPackets + \",\" + mylostPackets + \",\" + mytimeForwarded + \",\" + mythroughput + \");\"  );");
  allMetrics.push_back("	query( packetDataQuery.c_str() );");
  allMetrics.push_back("");
  allMetrics.push_back("	// calculate performance data and complete performance data table");
  allMetrics.push_back("	// throughtput = mythroughput");
  allMetrics.push_back("	// average flow delay = myDelaySum/myrxPackets");
  allMetrics.push_back("	// reliability = mytxPackets/myrxPackets");
  allMetrics.push_back("  	std::string myStatus = \"Ok\";");
  allMetrics.push_back("	double actualReliability;");
  allMetrics.push_back("	double actualDelay;");
  allMetrics.push_back("        if (rxPackets == 0)");
  allMetrics.push_back("          {");
  allMetrics.push_back("          actualReliability = 0;");
  allMetrics.push_back("          actualDelay = 0;");
  allMetrics.push_back("          }");
  allMetrics.push_back("        else");
  allMetrics.push_back("          {");
  allMetrics.push_back("          actualReliability = txPackets/rxPackets;");
  allMetrics.push_back("          actualDelay = delaySum/rxPackets;");
  allMetrics.push_back("          }");
  allMetrics.push_back("	string myactualReliability = NumberToString(actualReliability);");
  allMetrics.push_back("	string myactualDelay = NumberToString(actualDelay);");
  allMetrics.push_back("");
  allMetrics.push_back("	string myExpectedReliability = NumberToString(0.98);");
  allMetrics.push_back("	string myExpectedDelay = NumberToString(15);");
  allMetrics.push_back("	string myCost = NumberToString(0);");
  allMetrics.push_back("	string perfDataQuery (\"INSERT INTO performanceData VALUES(\"\"'\" + modelName + \"'\" + \", \" + myFlowIdNumber + \",\" + \"'\" + myStatus + \"'\" + \",\" + \"'\" + mySourceName + \"'\" + \",\" + \"'\" + myDestName + \"'\" + \",\" + \"'\" + myExpectedReliability + \"'\" + \",\" + \"'\" + myExpectedDelay + \"'\" + \",\" + myactualReliability + \",\" + myactualDelay + \",\" + mytxPackets + \",\" + myrxPackets + \",\" + mythroughput + \",\" + \"'\" + myCost + \"'\" + \");\"  );");
  allMetrics.push_back("	query( perfDataQuery.c_str() );");
  allMetrics.push_back("");
  allMetrics.push_back("");

  allMetrics.push_back("  }");
  allMetrics.push_back(" ");

  std::vector<std::string> flowBuild = GenerateFlowCpp();
  for(size_t i = 0; i <  flowBuild.size(); i++)
  {
    allMetrics.push_back("  " + flowBuild.at(i));
  }

  allMetrics.push_back(" ");
  allMetrics.push_back("  cout << \" \" << endl;");
  allMetrics.push_back("  cout << \" \" << endl;");
  allMetrics.push_back("  cout << \"================================================\" << endl;");
  allMetrics.push_back("  cout << \"=================Summary========================\" << endl;");
  allMetrics.push_back("  cout << \"================================================\" << endl;");
  allMetrics.push_back("  cout << \"All Tx Packets: \" << txPacketsum << endl;");
  allMetrics.push_back("  cout << \"All Rx Packets: \" << rxPacketsum << endl;");
  allMetrics.push_back("  cout << \"All Delay: \" << Delaysum / txPacketsum << endl;");
  allMetrics.push_back("  cout << \"All Lost Packets: \" << LostPacketsum << endl;");
  allMetrics.push_back("  cout << \"All Drop Packets: \" << DropPacketsum << endl;");
  allMetrics.push_back("");
  allMetrics.push_back("  if (txPacketsum != 0)");
  allMetrics.push_back("  {");
  allMetrics.push_back("    cout << \"Packets Delivery Ratio: \" << ((rxPacketsum * 100) / txPacketsum) << \"%\" << endl;");
  allMetrics.push_back("    cout << \"Packets Lost Ratio: \" << ((LostPacketsum * 100) / txPacketsum) << \"%\" << endl;");
  allMetrics.push_back("  }");
  allMetrics.push_back("");
  allMetrics.push_back("  	double newLostPacketsum = LostPacketsum/txPacketsum;");
  allMetrics.push_back("    // Summary information for all packet flows");
  allMetrics.push_back("	string mytx = NumberToString(txPacketsum);");
  allMetrics.push_back("	string myrx = NumberToString(rxPacketsum);");
  allMetrics.push_back("	string myDelay = NumberToString(Delaysum / txPacketsum);");
  allMetrics.push_back("	string myLost = NumberToString(LostPacketsum);");
  allMetrics.push_back("	string myDrop = NumberToString(DropPacketsum);");
  allMetrics.push_back("	string myDelRatio = NumberToString((rxPacketsum) / txPacketsum);");
  allMetrics.push_back(" 	string myLostRatio = NumberToString(newLostPacketsum);");
  allMetrics.push_back("");
  allMetrics.push_back("	query(\"CREATE TABLE if not exists summary (ModelName VARCHAR (20), TxPackets STRING, RxPackets STRING, Delay STRING, LostPackets STRING, DroppedPackets STRING, PacketDeliveryRatio STRING, PacketLostRatio STRING);\");");
  allMetrics.push_back("	string mysqlquery (\"INSERT INTO summary VALUES(\"\"'\" + modelName + \"'\" + \", \" + mytx + \", \" + myrx + \", \" + myDelay + \", \" + myLost + \", \" + myDrop + \", \" + myDelRatio + \", \" + myLostRatio + \");\"  );");
  allMetrics.push_back("	query( mysqlquery.c_str() );");
  allMetrics.push_back("");
  allMetrics.push_back("	// close the database");
  allMetrics.push_back("	sqlite3_close(database);");

  return allMetrics;
}

//
// Cpp generation operation part.
//

void Generator::WriteCpp(const std::string &line) 
{
//std::cout << line << std::endl;
  if(this->m_cppFile.is_open())
  {
    this->m_cppFile << line + '\n';
  }
}
