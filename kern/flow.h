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
 * \file flow.h
 * \brief Flow base class.
 * \author Jeffrey Young
 * \date 2016
 */

#ifndef FLOW_H
#define FLOW_H

#include <iostream>
#include <string>
#include <vector>

/**
 * \ingroup generator
 * \brief Flow base class.
 *
 *  The flow class represent the flows.
 *  You can create here différents flow type :
 *    - wifi
 *    - multiHop
 *    - wired
 *
 */
class Flow 
{
  private:
    /**
     * \brief Flow number.
     */
    size_t m_indice;

    /**
     * \brief Model name.
     */
    std::string m_modelName;

    /**
     * \brief Flow name.
     */
    std::string m_flowName;

    /**
     * \brief Flow source.
     */
    std::string m_source;

    /**
     * \brief Flow source.
     */
    std::string m_destination;

    /**
     * \brief Flow source.
     */
    std::string m_expectedDelay;

    /**
     * \brief Flow source.
     */
    std::string m_expectedReliability;

    /**
     * \brief Ip interface name. 
     */
    std::string m_ipInterfaceName;

    /**
     * \brief Flow type.
     */
    std::string m_type;

  public:
    /**
     * \brief Constructor.
     * \param indice number of the flow
     * \param type type of the flow (pc, router, ...)
     * \param namePrefix the prefix name of the flow
     * \param machinesNumber number of machine to create
     */
    Flow(const size_t &indice, const std::string &type, const std::string &namePrefix, const std::string &source, const std::string &destination, const std::string &expectedDelay, const std::string &expectedReliability);

    /**
     * \brief Destructor.
     */
    ~Flow();

    /**
     * \brief Generate headers code.
     * This function return a vector which contain the header lines from the specified object
     * \return headers code
     */
    std::vector<std::string> GenerateHeader();

    /**
     * \brief Generate flow C++ code.
     * This function return a vector which contain the declaration and instanciation of the flow
     * \return flow code
     */
    std::vector<std::string> GenerateFlowCpp();

    /**
     * \brief Generate Flowmonitor install C++ code.
     * This function returns a vector which contains the declaration of Flowmonitor on a node
     * \return Flowmonitor declaration code
     */
    std::vector<std::string> GenerateFlowmonitor();

    /**
     * \brief Set flow name.
     * \param flowName flow name
     */
    void SetFlowName(const std::string &flowName);

    /**
     * \brief Set ip interface name.
     *
     * This procedure is used to the the ipInterfaceName.
     * Sometimes this var is used in application like as UdpEcho.
     * \param ipInterfaceName ip interface name
     */
    void SetIpInterfaceName(const std::string &ipInterfaceName);

    /**
     * \brief Get flow name.
     * \return flow name
     */
    std::string GetFlowName();

    /**
     * \brief Get flow name.
     * \param number machine number
     * \return flow name
     */
    std::string GetFlowName(const size_t &number);

    /**
     * \brief Get ip interface name.
     * \return IP interface name
     */
    std::string GetIpInterfaceName();

    /**
     * \brief Get indice.
     * \return indice
     */
    std::string GetIndice();

    /**
     * \brief Set indice.
     * \param indice new indice
     */
    void SetIndice(const size_t &indice);

    /**
     * \brief Get flow type.
     * \return type the flow type
     */
    std::string GetFlowType();
};

#endif /* FLOW_H */
