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
 * \file demand-response.h
 * \brief The demand response class is subclass of Application.
 * \author Jeffrey Young
 * \date 2015
 */

#ifndef DEMAND_RESPONSE_H
#define DEMAND_RESPONSE_H

#include "application.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * \ingroup generator
 * \brief The demand response subclass from Application.
 *
 *  The demand-response class is a subclass of application.
 *
 *  This class represent a meter data management application.
 *
 *  It re-writte some methods which are called to generate the ns3 code.
 *
 *  Usage:
 *    - Create an application with demand-response type.
 */
class DemandResponse : public Application
{
  public:
    /**
     * \brief Constructor.
     * \param type application type
     * \param indice indice in the generator vector
     * \param senderNode sender node
     * \param receiverNode receiver node
     * \param startTime application start time
     * \param endTime application end time
     */
    DemandResponse(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime);

    /**
     * \brief Destructor.
     */
    ~DemandResponse();

    /**
     * \brief Generate the headers code.
     * \return headers code
     */
    virtual std::vector<std::string> GenerateHeader();

    /**
     * \brief Generate the application C++ code.
     * \param netDeviceContainer net device container name which are contain the sender node
     * \param numberIntoNetDevice place of the machine into the net device container
     * \return application code
     */
    virtual std::vector<std::string> GenerateApplicationCpp(std::string netDeviceContainer, size_t numberIntoNetDevice);
    
    /**
     * \brief Generate the application python code.
     * \param netDeviceContainer net device container name which are contain the sender node
     * \param numberIntoNetDevice place of the machine into the net device container
     * \return application code
     */
    virtual std::vector<std::string> GenerateApplicationPython(std::string netDeviceContainer, size_t numberIntoNetDevice);
};

#endif /* DEMAND_RESPONSE_H */
