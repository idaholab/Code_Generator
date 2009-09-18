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
 * \file drag-lines.h
 * \brief The dragged lines.
 * \author Pierre Weiss
 * \date 2009
 */

#ifndef DRAG_LINES_H
#define DRAG_LINES_H

#include <vector>
#include <QtGui>

/**
 * \ingroup generator
 * \brief The dragged lines.
 *
 * This class represent the coordonate to be drawed.
 */
class DragLines
{
  public:
    /**
     * \brief Constructor.
     */
    DragLines();

    /**
     * \brief Begin.
     */
    std::string begin;

    /**
     * \brief End.
     */
    std::string end;
    
    /**
     * \brief Type.
     */
    std::string type;
};

#endif /* DRAG_LINES_H */

