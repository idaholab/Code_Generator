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
 * \file meter-data-management.h
 * \brief The meter data management class is subclass of Application.
 * \author Jeffrey Young
 * \date 2015
 */

#ifndef METER_DATA_MANAGEMENT_H
#define METER_DATA_MANAGEMENT_H

#include "application.h"

#include <iostream>
#include <string>
#include <vector>

/**
 * \ingroup generator
 * \brief The meter data management subclass from Application.
 *
 *  The meter-data-management class is a subclass of application.
 *
 *  This class represent a meter data management application.
 *
 *  It re-writte some methods which are called to generate the ns3 code.
 *
 *  Usage:
 *    - Create an application with meter-data-management type.
 */
class MeterDataManagement : public Application
{
  private:
    /**
     * \brief interval
     */
    size_t m_interval;

    /**
     * \brief readingTime
     */
    size_t m_readingTime;

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
    MeterDataManagement(const std::string &type, const size_t &indice, const std::string &senderNode, const std::string &receiverNode, const size_t &startTime, const size_t &endTime, const size_t &interval, const size_t &readingTime);

    /**
     * \brief Destructor.
     */
    ~MeterDataManagement();

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
     * \brief Get interval.
     * \return interval
     */
    std::string GetInterval();

    /**
     * \brief Set interval.
     * \param interval new interval
     */
    void SetInterval(const size_t &interval);

    /**
     * \brief Get readingTime.
     * \return readingTime
     */
    std::string GetReadingTime();

    /**
     * \brief Set readingTime.
     * \param readingTime new readingTime
     */
    void SetReadingTime(const size_t &readingTime);

};

#endif /* METER_DATA_MANAGEMENT_H */
