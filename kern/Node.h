/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 Pierre Weiss <3weissp@gmail.com>
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
 */
/**
* \file Node.h
* \brief Node Base Class.
* \author Pierre Weiss
* \date 2009
*/

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>


/**
 * \ingroup generator
 * \brief Node Base Class.
 *
 * This class is the main class of the differents equipement you can choose to use.
 * It contain the Pc Switch, Router, Tap, Wifi Ap, Wifi Station.
 */
class Node 
{

private:
  /**
   * \brief attribute which represent the node number.
   */
  size_t indice;
  
  /**
   * \brief attribute which represent the node name.
   */
  std::string nodeName;
  
  /**
   * \brief attribute which represent the ip interface name. 
   */
  std::string ipInterfaceName;
  
  /**
   * \brief attribute which represent the nsc use.
   */
  std::string nsc;
  
  /**
   * \brief attribute which represent the number of machines.
   */
  size_t machinesNumber;
  
public:
  
  /**
   * \brief Constructor which set default value.
   * \param _indice
   * \param _type
   * \param _machinesNumber
   */
  Node(const size_t &_indice, const std::string &_type, const size_t &_machinesNumber);
  
  /**
   * \brief Destructor from the class Node.
   */
  ~Node();

  /**
   * \brief Function wich return the c++ header code from node.
   *
   * This function return a string which contain the header lines from the specified object.
   * 
   * \return header.
   */
  std::vector<std::string> GenerateHeader();

  /**
   * \brief Function wich return the c++ code from node.
   *
   * This function return a string which contain the declaration and instanciation of the node.
   * 
   * \return node.
   */
  std::vector<std::string> GenerateNode();

  /**
   * \brief Function wich return the c++ code from IpStack.
   *
   * This function return a string which contain the c++ code from the Ipv4 stack declaration and instanciation.
   * 
   * \return ipStack.
   */
  std::vector<std::string> GenerateIpStack();
  
  /**
   * \brief Procedure to set the node name.
   * 
   * \param _nodeName the string which containe the node name.
   */
  void setNodeName(const std::string &_nodeName);

  
  /**
   * \brief Procedure to set the Ip interface name.
   * 
   * This procedure is used to the the ipInterfaceName. 
   * Sometimes this var is used in application like as UdpEcho.
   *
   * \param _ipInterfaceName
   */
  void setIpInterfaceName(const std::string &_ipInterfaceName);
  
  /**
   * \brief Function wich return the node name.
   *
   * \return node name.
   */
  std::string getNodeName();
  
  /**
   * \brief Function wich return the node name.
   * \param number
   * \return node name.
   */
  std::string getNodeName(const size_t &number);

  /**
   * \brief Function wich return the name of the Ip interface.
   * 
   * \return ip interface name.
   */
  std::string getIpInterfaceName();

  /**
  * \brief Function wich return the node number.
  *
  * \return node number in type string.
  */
  std::string getIndice();
  
  /**
   * \brief Function to get the nsc use.
   * \return string
   */
  std::string getNsc();
  
  /**
   * \brief Procedure to set the nsc use.
   * \param _nsc
   */
  void setNsc(const std::string &_nsc);
  
  /**
   * \brief get the number of machines container un the node.
   * \return number
   */
  size_t getMachinesNumber();

  /**
   * \brief procedure to set the machines number.
   * \param _machinesNumber
   */
  void setMachinesNumber(const size_t &_machinesNumber);
  
};

#endif /* NODE_H */
