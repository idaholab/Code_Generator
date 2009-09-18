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
 * \file generator.h
 * \brief The main class of the Generator.
 * \author Pierre Weiss
 * \date 2009
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include "node.h"
#include "link.h"
#include "application.h"


#include <iostream>
#include <string>
#include <vector>

/**
 * \ingroup generator
 * \brief The main class of the Generator.
 *
 *  The Generator class is the main class of the kern folder.
 *  It is used for call all the classes you can found here.
 *  
 *  This class works around three vector which contain Nodes, Links and Applications.
 *  
 *  Usage example:
 *    - create an Generator object,
 *      Generator *gen = new Generator(std::string("Simulation Name"));
 *    - add nodes,
 *      gen->AddNode(std::string("Pc"));// or router, wifi station, ...
 *    - add links,
 *      gen->AddLink(std::string("Hub"));// or ap, bridge, ....
 *    - connect nodes to link,
 *      gen->(link)->Install(Node name);
 *    - add applications,
 *      gen->AddApplication(std::string("Ping"), senderNodeName, receiverNodeName, startTime, endTime);
 *
 *    You can see all examples of all implementation into kern/examples/ folder.
 */
class Generator
{
  public:
    /**
     * \brief Constructor.
     * \param simulationName simulation name
     */
    Generator(const std::string &simulationName);

    /**
     * \brief Destructor.
     */
    ~Generator();	

    /**
     * \brief Add a config line.
     * \param config new config to add to config list
     */
    void AddConfig(const std::string &config);

    /**
     * \brief Add an node.
     * 
     * This used to add an node. It add the node to the
     * vector listNode and increment the number of node.
     * 
     * \param type node type. (pc, hub, switch,...)
     */
    void AddNode(const std::string &type);

    /**
     * \brief Add an node group.
     * \param type node type
     * \param number number which will be added
     */
    void AddNode(const std::string &type, const size_t number);

    /**
     * \brief Remove an node element.
     * \param name node name to remove
     */
    void RemoveNode(const std::string &name);

    /**
     * \brief Remove an node element.
     * \param index node index to be removed
     */
    void RemoveNode(const size_t index);

    /**
     * \brief Get node at specified index.
     * \param index index
     * \return node pointer
     */
    Node* GetNode(const size_t index);

    /**
     * \brief Get number of nodes.
     * \return number of nodes.
     */
    size_t GetNNodes() const;

    /**
     * \brief Add an appplication.
     * 
     * This procedure is used to add an procedure. It add the application to the
     * vector listApplication and increment the number of application.
     * 
     * \param type type of the application. (ping, tcp-large-transfert, udp-echo ...)
     * \param senderNode sender node
     * \param receiverNode receiver node
     * \param startTime start time
     * \param endTime end time
     * \param port port
     */
    void AddApplication(const std::string &type, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &port);

    /**
     * \brief Add an application.
     * 
     * This procedure is used to add an procedure. It add the application to the
     * vector listApplication and increment the number of application.
     * 
     * \param type the type of the application. (ping, tcp-large-transfert, udp-echo ...)
     * \param senderNode sender node
     * \param receiverNode receiver node
     * \param startTime start time
     * \param endTime end time
     */
    void AddApplication(const std::string &type, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime);

    /**
     * \brief Remove an application element.
     * \param name application name to remove
     */
    void RemoveApplication(const std::string &name);

    /**
     * \brief Get application element at specified index.
     * \param index index
     * \return application pointer
     */
    Application* GetApplication(const size_t index);

    /**
     * \brief Get number of applications.
     * \return number of applications
     */
    size_t GetNApplications() const; 

    /**
     * \brief Add an link.
     * 
     * This procedure is used to add an link. It add the link to the
     * vector listLink and increment the number of link.
     * 
     * \param type type of the link. (point-to-point, csma,...)
     */
    void AddLink(const std::string &type);

    /**
     * \brief Add link and his linkNode.
     * 
     *  Example, an Ap, must have an ap node to be generated.  
     *
     * \param type link type
     * \param linkNode node name
     */
    void AddLink(const std::string &type, const std::string &linkNode);

    /**
     * \brief Add link with her linkNode and ifaceName.
     * 
     * \param type link type
     * \param linkNode node name
     * \param ifaceName iface name
     */
    void AddLink(const std::string &type, const std::string &linkNode, const std::string &ifaceName);

    /**
     * \brief Remove an link element.
     * \param name link name to remove
     */
    void RemoveLink(const std::string &name);

    /**
     * \brief Remove an link element.
     * \param index index of the link to remove
     */
    void RemoveLink(const size_t index);

    /**
     * \brief Get link at specified index.
     * \param index index
     * \return link pointer
     */
    Link* GetLink(const size_t index);

    /**
     * \brief Get number of links.
     * \return number of links
     */
    size_t GetNLinks() const;

    /**
     * \brief Fenerate ns3 code.
     * 
     * This procedure is the main procedure to generate the code from the simulation.
     * It also go to use all the procedure on the bottom like generate all headers, ...
     */
    void GenerateCode();

  private:
    /**
     * \brief Simulation name.
     */
    std::string m_simulationName;

    /**
     * \brief Configuration list.
     */
    std::vector<std::string> listConfiguration;

    /**
     * \brief Number attribute of term node created.
     */
    size_t m_indiceNodePc;

    /**
     * \brief Number attribute of router node created.
     */
    size_t m_indiceNodeRouter;

    /**
     * \brief Number attribute of ap node created.
     */
    size_t m_indiceNodeAp;

    /**
     * \brief Number attribute of station node created.
     */
    size_t m_indiceNodeStation;

    /**
     * \brief Number attribute of bridge node created.
     */
    size_t m_indiceNodeBridge;

    /**
     * \brief Number attribute of tap node created.
     */
    size_t m_indiceNodeTap;

    /**
     * \brief Number attribute of emu node created.
     */
    size_t m_indiceNodeEmu;

    /**
     * \brief List of node created instance.
     * 
     * This attribute is the list of the created instance of node.
     */
    std::vector<Node*> listNode;

    /**
     * \brief List of application created instance.
     * 
     * This attribute is the list of the created instance of application.
     */
    std::vector<Application*> listApplication;

    /**
     * \brief List of link created instace.
     * 
     * This attribute is the list of the created instance of link.
     */
    std::vector<Link*> listLink;

    /**
     * \brief Number attribute of ping application created.
     */
    size_t m_indiceApplicationPing; 

    /**
     * \brief Number attribute of tcp large transfert application created.
     */
    size_t m_indiceApplicationTcpLargeTransfer;

    /**
     * \brief Number attribute of udp echo application created.
     */
    size_t m_indiceApplicationUdpEcho;

    /**
     * \brief Number attribute of ap link created.
     */
    size_t m_indiceLinkAp;

    /**
     * \brief Number attribute of emu link created.
     */
    size_t m_indiceLinkEmu;

    /**
     * \brief Number attribute of point-to-point link created.
     */
    size_t m_indiceLinkPointToPoint;

    /**
     * \brief Number attribute of tap link created.
     */
    size_t m_indiceLinkTap;

    /**
     * \brief Number attribute of hub link created.
     */
    size_t m_indiceLinkHub;

    /**
     * \brief Number attribute of bridge link created.
     */
    size_t m_indiceLinkBridge; 

    //
    // Code generation operation part
    //

    /**
     * \brief Generate headers code.
     * 
     * This function return a string wich contain all c++ header without duplicate.
     * 
     * \return headers code
     */
    std::vector<std::string> GenerateHeader();

    /**
     * \brief Generate vars code.
     * 
     * This function return a string wich contain all c++ vars.
     * 
     * \return vars code
     */
    std::vector<std::string> GenerateVars();

    /**
     * \brief Generate command line code.
     * 
     * \return commend line code
     */
    std::vector<std::string> GenerateCmdLine();

    /**
     * \brief Generate config code.
     * 
     * \return config code
     */
    std::vector<std::string> GenerateConfig();

    /**
     * \brief Generate node declaration code.
     *
     * \return declaration code
     */
    std::vector<std::string> GenerateNode();

    /**
     * \brief Generate link code.
     * 
     * \return link code
     */
    std::vector<std::string> GenerateLink();

    /**
     * \brief Generate net device code.
     * 
     * \return net device code
     */
    std::vector<std::string> GenerateNetDevice();

    /**
     * \brief Generate ip stack declaration.
     * 
     * \return IP stack code
     */
    std::vector<std::string> GenerateIpStack();

    /**
     * \brief Generate ip assign code.
     * 
     * \return IP assign code
     */
    std::vector<std::string> GenerateIpAssign();

    /**
     * \brief Generate tap bridge code.
     * 
     * \return tab bridge code
     */
    std::vector<std::string> GenerateTapBridge();

    /**
     * \brief Generate route code.
     * 
     * \return route code
     */
    std::vector<std::string> GenerateRoute();

    /**
     * \brief Generate application code.
     * 
     * \return application code
     */
    std::vector<std::string> GenerateApplication();

    /**
     * \brief Generate trace code.
     * 
     * \return trace code
     */
    std::vector<std::string> GenerateTrace();

    //
    // XML generation operation part.
    //

    /**
     * \brief XML file name atribute.
     * 
     * This file name is used to export the current simulation into XML to save it.
     */
    std::string m_xmlFileName;

    /**
     * \brief Get XML file name.
     *
     * \return xml file name
     */
    std::string getXmlFileName();

    /**
     * \brief Set xml file name.
     *
     * \param xmlFileName the new XML file name
     */
    void setXmlFileName(const std::string &xmlFileName );

    /**
     * \brief Write simulation into XML file.
     * 
     * \param line the line to add to the XML file
     */
    void WriteXml(const std::string &line);

    /**
     * \brief Open and print graphicaly the XML file.
     */
    void OpenXml();

    //
    // C++ generation operation part.
    //

    /**
     * \brief Name of the C++ output file name.
     */  
    std::string m_cppFileName;

    /**
     * \brief Get the C++ output file name.
     * \return C++ file name
     */
    std::string getCppFileName();

    /**
     * \brief Set C++ output file name.
     * \param cppFileName the new C++ file name
     */
    void setCppFileName(const std::string &cppFileName);

    /**
     * \brief Write C++ code into the attribute file name.
     * \param line the new line to add to the attribute file name
     */
    void WriteCpp(const std::string &line);

    //
    // Python generation operation part.
    //

    /**
     * \brief Name of the python output file name.
     */
    std::string m_pyFileName;

    /**
     * \brief Get python output file name.
     * \return python file name
     */
    std::string getPyFileName();

    /**
     * \brief Set the python output file name.
     * \param pyFileName the new python file name
     */
    void setPyFileName(const std::string &pyFileName);

    /**
     * \brief Write python code into the output file name.
     * \param line the line to be writted 
     */
    void WritePython(const std::string &line);
};

#endif /* GENERATOR_H */

