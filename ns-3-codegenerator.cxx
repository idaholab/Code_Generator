// file      : examples/cxx/tree/codegenerator/ns-3-codegenerator.cxx
// copyright : not copyrighted - public domain

using namespace std;

#include <memory>   // std::auto_ptr
#include <iostream>
#include <cstdlib>

#include "TopologySchema.hxx"

#include "kern/generator.h"
#include "kern/ap.h"
#include "kern/udp-echo.h"
#include "kern/udp-cosem.h"
#include "kern/data-concentrator.h"
#include "kern/meter-data-management.h"

///using std::cerr;
///using std::endl;

int
main (int argc, char* argv[])
{
  /* avoid compile warnings */
  argc = argc;
  argv = argv;
  
  if (argc != 3)
  {
    cerr << "usage: " << argv[0] << " <file>.xml <filename>.cc" << endl;
    return 1;
  }

  try
  {
    ///using namespace AMISimpleWireless;

    // Read in the XML file and obtain its object model.
    //

  Generator *gen = new Generator(std::string(argv[1]));

xml_schema::properties properties;
properties.no_namespace_schema_location("TopologySchema.xsd");

    std::auto_ptr<Gen> g = Gen_ (argv[1], 0, properties);

    // Let's print what we've got.
    //

/// Build nodes ///
int i = 0;
Gen::Nodes_type::node_const_iterator ni = g->Nodes().node().begin();
for (; ni != g->Nodes().node().end(); ni++)
  {
cerr << "gen->AddNode(" << (*ni).type() << ", " << (*ni).name() << ")" << endl;
  gen->AddNode((*ni).type(), (*ni).name());
cerr << "gem->GetNode(" << i << ")->SetFlowmonitor(" << (*ni).enableFlowmonitor() << ")" << endl;
  if ((*ni).enableFlowmonitor() == "true")
    gen->GetNode(i)->SetFlowmonitor(true);
  else gen->GetNode(i)->SetFlowmonitor(false);
  i++;
  }

/// Add Network hardware ///
///  Ap *ap = dynamic_cast<Ap*>(gen->GetNetworkHardware(0));
char buffer[20];
std::size_t length;

i = 0;
Gen::NetworkHardwares_type::networkHardware_const_iterator hi = g->NetworkHardwares().networkHardware().begin();
Gen::NetworkHardwares_type::networkHardware_type::connectedNodes_type::name_const_iterator namei;
for (; hi != g->NetworkHardwares().networkHardware().end(); hi++)
  {
  if (((*hi).type() == "Ap") || ((*hi).type() == "Bridge"))
    {
    if (((*hi).name().compare(0, 2, "hw") != 0) && ((*hi).type() == "Ap"))
      {
      cerr << "Error-> Improper AP hardware name <" << (*hi).name() << ">" << endl;
      return 1;
      }

    length = (*hi).name().copy(buffer, (*hi).name().length(), 2);  //Trim off the first two characters
    buffer[length] = '\0';

cerr << "gen->AddNetworkHardware(" << (*hi).type() << ", " << buffer << ")" << endl;
    gen->AddNetworkHardware((*hi).type(), buffer);

cerr << "gen->GetNetworkHardware(" << i << ")->SetDataRate(" << (*hi).dataRate() << ")" << endl;
    gen->GetNetworkHardware(i)->SetDataRate((*hi).dataRate());
cerr << "gen->GetNetworkHardware(" << i << ")->SetNetworkHardwareDelay(" << (*hi).linkDelay() << ")" << endl;
    gen->GetNetworkHardware(i)->SetNetworkHardwareDelay((*hi).linkDelay());
cerr << "gen->GetNetworkHardware(" << i << ")->SetTrace(" << (*hi).enableTrace() << ")" << endl;
    if ((*hi).enableTrace() == "true")
      gen->GetNetworkHardware(i)->SetTrace(true);
    else gen->GetNetworkHardware(i)->SetTrace(false);

    }
  else
    {
cerr << "gen->AddNetworkHardware(" << (*hi).type() << ")" << endl;
    gen->AddNetworkHardware((*hi).type());

cerr << "gen->GetNetworkHardware(" << i << ")->SetDataRate(" << (*hi).dataRate() << ")" << endl;
    gen->GetNetworkHardware(i)->SetDataRate((*hi).dataRate());
cerr << "gen->GetNetworkHardware(" << i << ")->SetNetworkHardwareDelay(" << (*hi).linkDelay() << ")" << endl;
    gen->GetNetworkHardware(i)->SetNetworkHardwareDelay((*hi).linkDelay());
cerr << "gen->GetNetworkHardware(" << i << ")->SetTrace(" << (*hi).enableTrace() << ")" << endl;
    if ((*hi).enableTrace() == "true")
      gen->GetNetworkHardware(i)->SetTrace(true);
    else gen->GetNetworkHardware(i)->SetTrace(false);

    }

    for (namei = (*hi).connectedNodes().name().begin(); namei != (*hi).connectedNodes().name().end(); namei++)
    {
cerr << "gen->GetNetworkHardware(" << i << ")->Install(" << *namei << ")" << endl;
    gen->GetNetworkHardware(i)->Install(*namei);
    }

  i++;
  }

/// Add Applications ///
i = 0;
///int j = 0;
Gen::Applications_type::application_const_iterator ai = g->Applications().application().begin();
for (; ai != g->Applications().application().end(); ai++)
  {
///UDP Echo
  if ((*ai).type() == "UdpEcho")
    {
cerr << "gen->AddApplication(" << (*ai).type() << ", " << (*ai).sender() << ", " << (*ai).receiver() << ", " << (*ai).startTime() << ", " << (*ai).endTime() << ", " << *ai->special()->port() << ")" << endl;

    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime(), *ai->special()->port());

    UdpEcho *app = dynamic_cast<UdpEcho*>(gen->GetApplication(i));
    app->SetPacketSize(*ai->special()->packetSize());
    app->SetPacketIntervalTime(*ai->special()->packetIntervalTime());
    app->SetMaxPacketCount(*ai->special()->maxPacketCount());

    i++;
    }

///UDP cosem
  if ((*ai).type() == "UdpCosem")
    {
cerr << "gen->AddApplication(" << (*ai).type() << ", " << (*ai).sender() << ", " << (*ai).receiver() << ", " << (*ai).startTime() << ", " << (*ai).endTime() << ", " << *ai->special()->port() << ")" << endl;

    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime(), *ai->special()->port());

    UdpCosem *appCosem = dynamic_cast<UdpCosem*>(gen->GetApplication(i));
///    appCosem->SetPacketSize(*ai->special()->packetSize());
    appCosem->SetPacketIntervalTime(*ai->special()->packetIntervalTime());
///    appCosem->SetMaxPacketCount(*ai->special()->maxPacketCount());


    i++;
    }

///Data Concentrator
  if ((*ai).type() == "DataConcentrator")
    {
cerr << "gen->AddApplication(" << (*ai).type() << ", " << (*ai).sender() << ", " << (*ai).receiver() << ", " << (*ai).startTime() << ", " << (*ai).endTime() << ")" << endl;

    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime());

///    DataConcentrator *appDC = dynamic_cast<UdpDC*>(gen->GetApplication(i));
///    appCosem->SetPacketSize(*ai->special()->packetSize());
///    appDC->SetPacketIntervalTime(*ai->special()->packetIntervalTime());
///    appCosem->SetMaxPacketCount(*ai->special()->maxPacketCount());


    i++;
    }

///Meter Data Management
  if ((*ai).type() == "MeterDataManagement")
    {
cerr << "gen->AddApplication(" << (*ai).type() << ", " << (*ai).sender() << ", " << (*ai).receiver() << ", " << (*ai).startTime() << ", " << (*ai).endTime() << ", " << *ai->special()->interval() << ", " << *ai->special()->readingTime() << ")" << endl;

    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime(), *ai->special()->interval(), *ai->special()->readingTime());

    i++;
    }

///Demand Response
  if ((*ai).type() == "DemandResponse")
    {
cerr << "gen->AddApplication(" << (*ai).type() << ", " << (*ai).sender() << ", " << (*ai).receiver() << ", " << (*ai).startTime() << ", " << (*ai).endTime() << ")" << endl;

    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime());

    i++;
    }

///TCP transfer
  if ((*ai).type() == "TcpLargeTransfer")
    {
    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime(), *ai->special()->port());
    }

///Ping
  if ((*ai).type() == "Ping")
    {
    gen->AddApplication((*ai).type(), (*ai).sender(), (*ai).receiver(), (*ai).startTime(), (*ai).endTime());
    }

  }

/// Generate ns-3 cc file ///
  gen->GenerateCodeCpp(argv[2]);
  delete gen;

  }
  catch (const xml_schema::exception& e)
  {
    cerr << e << endl;
    return 1;
  }
}
