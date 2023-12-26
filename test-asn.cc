#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/packet-sink.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/netanim-module.h" // Include NetAnim
#include "ns3/mobility-module.h" // Include Mobility
#include "ns3/node-container.h"
#include <iostream>


using namespace ns3;

NetDeviceContainer GetDevicesInRange(NetDeviceContainer& devices, uint32_t startIndex, uint32_t endIndex) {
    // Create a temporary container to hold the devices in the specified range
    NetDeviceContainer tempDevices;

    // Iterate through the devices and add those within the range to the temporary container
    for (uint32_t i = startIndex; i <= endIndex; ++i) {
        tempDevices.Add(devices.Get(i));
    }

    // Return the temporary container containing the devices in the specified range
    return tempDevices;
}


int main() {
    // Initialize NS-3 environment
    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer nodes;
    nodes.Create(50);

    // Install network stack on nodes
    InternetStackHelper stack;
    stack.Install(nodes);

    // Connect nodes with point-to-point links
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps")); // Set a default data rate
    p2p.SetChannelAttribute("Delay", StringValue("2ms")); // Set a default delay

    NetDeviceContainer devices;
    for (uint32_t i = 0; i < nodes.GetN() - 1; ++i) {
        devices.Add(p2p.Install(nodes.Get(i), nodes.Get(i + 1)));
    }

    // Divide nodes into 5 ASes
    NodeContainer as1, as2, as3, as4, as5;
    for (uint32_t i = 0; i < 10; ++i) {
        as1.Add(nodes.Get(i)); // Add nodes 0 to 9 to AS1
    }
    for (uint32_t i = 10; i < 20; ++i) {
        as2.Add(nodes.Get(i)); // Add nodes 10 to 19 to AS2
    }
    for (uint32_t i = 20; i < 30; ++i) {
        as3.Add(nodes.Get(i)); // Add nodes 20 to 29 to AS3
    }
    for (uint32_t i = 30; i < 40; ++i) {
        as4.Add(nodes.Get(i)); // Add nodes 30 to 39 to AS4
    }
    for (uint32_t i = 40; i < 50; ++i) {
        as5.Add(nodes.Get(i)); // Add nodes 40 to 49 to AS5
    }

    // Designate border routers
    Ptr<Node> as1Border = as1.Get(0);
    Ptr<Node> as2Border = as2.Get(0);
    Ptr<Node> as3Border = as3.Get(0);
    Ptr<Node> as4Border = as4.Get(0);
    Ptr<Node> as5Border = as5.Get(0);

    // Configure interfaces and assign IP addresses for AS1
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    NetDeviceContainer as1Devices = GetDevicesInRange(devices, 0,9);
    Ipv4InterfaceContainer as1Interfaces = address.Assign(as1Devices);

        // Configure interfaces and assign IP addresses for AS2
    Ipv4AddressHelper address2;
    address2.SetBase("10.2.1.0", "255.255.255.0");
    NetDeviceContainer as2Devices = GetDevicesInRange(devices, 10,19);
    Ipv4InterfaceContainer as2Interfaces = address2.Assign(as2Devices);

    // Configure interfaces and assign IP addresses for AS3
    Ipv4AddressHelper address3;
    address3.SetBase("10.3.1.0", "255.255.255.0");
    NetDeviceContainer as3Devices = GetDevicesInRange(devices, 20,29);

    Ipv4InterfaceContainer as3Interfaces = address3.Assign(as3Devices);

    // Configure interfaces and assign IP addresses for AS4
    Ipv4AddressHelper address4;
    address4.SetBase("10.4.1.0", "255.255.255.0");
    NetDeviceContainer as4Devices = GetDevicesInRange(devices, 30,39);

    Ipv4InterfaceContainer as4Interfaces = address4.Assign(as4Devices);
    
    // Configure interfaces and assign IP addresses for AS5
    Ipv4AddressHelper address5;
    address5.SetBase("10.5.1.0", "255.255.255.0");
    //is this func gonna connect all the devices inside an arbritrary ASN??
    NetDeviceContainer as5Devices = GetDevicesInRange(devices, 40,49);
    
    Ipv4InterfaceContainer as5Interfaces = address5.Assign(as5Devices);



    // AS1 to AS2 link
    Ipv4AddressHelper addressAS1AS2;
    addressAS1AS2.SetBase("10.10.1.0", "255.255.255.0");
    NetDeviceContainer devicesAS1AS2 = p2p.Install(as1Border, as2Border);
    Ipv4InterfaceContainer interfacesAS1AS2 = addressAS1AS2.Assign(devicesAS1AS2);

    // AS1 to AS3 link
    Ipv4AddressHelper addressAS1AS3;
    addressAS1AS3.SetBase("10.10.2.0", "255.255.255.0");
    NetDeviceContainer devicesAS1AS3 = p2p.Install(as1Border, as3Border);
    Ipv4InterfaceContainer interfacesAS1AS3 = addressAS1AS3.Assign(devicesAS1AS3);

    // AS1 to AS4 link
    Ipv4AddressHelper addressAS1AS4;
    addressAS1AS4.SetBase("10.10.3.0", "255.255.255.0");
    NetDeviceContainer devicesAS1AS4 = p2p.Install(as1Border, as4Border);
    Ipv4InterfaceContainer interfacesAS1AS4 = addressAS1AS4.Assign(devicesAS1AS4);

    // AS1 to AS5 link
    Ipv4AddressHelper addressAS1AS5;
    addressAS1AS5.SetBase("10.10.4.0", "255.255.255.0");
    NetDeviceContainer devicesAS1AS5 = p2p.Install(as1Border, as5Border);
    Ipv4InterfaceContainer interfacesAS1AS5 = addressAS1AS5.Assign(devicesAS1AS5);

    // AS2 to AS3 link
    Ipv4AddressHelper addressAS2AS3;
    addressAS2AS3.SetBase("10.10.5.0", "255.255.255.0");
    NetDeviceContainer devicesAS2AS3 = p2p.Install(as2Border, as3Border);
    Ipv4InterfaceContainer interfacesAS2AS3 = addressAS2AS3.Assign(devicesAS2AS3);

    // AS2 to AS4 link
    Ipv4AddressHelper addressAS2AS4;
    addressAS2AS4.SetBase("10.10.6.0", "255.255.255.0");
    NetDeviceContainer devicesAS2AS4 = p2p.Install(as2Border, as4Border);
    Ipv4InterfaceContainer interfacesAS2AS4 = addressAS2AS4.Assign(devicesAS2AS4);

    // AS3 to AS4 link
    Ipv4AddressHelper addressAS3AS4;
    addressAS3AS4.SetBase("10.10.7.0", "255.255.255.0");
    NetDeviceContainer devicesAS3AS4 = p2p.Install(as3Border, as4Border);
    Ipv4InterfaceContainer interfacesAS3AS4 = addressAS3AS4.Assign(devicesAS3AS4);

    // AS3 to AS5 link
    Ipv4AddressHelper addressAS3AS5;
    addressAS3AS5.SetBase("10.10.8.0", "255.255.255.0");
    NetDeviceContainer devicesAS3AS5 = p2p.Install(as3Border, as5Border);
    Ipv4InterfaceContainer interfacesAS3AS5 = addressAS3AS5.Assign(devicesAS3AS5);

    // AS5 to AS4 link
    Ipv4AddressHelper addressAS5AS4;
    addressAS5AS4.SetBase("10.10.9.0", "255.255.255.0");
    NetDeviceContainer devicesAS5AS4 = p2p.Install(as5Border, as4Border);
    Ipv4InterfaceContainer interfacesAS5AS4 = addressAS5AS4.Assign(devicesAS5AS4);




    // Create animation object and output the animation to testing.xml
    AnimationInterface anim("testing.xml");
    anim.SetConstantPosition(as1.Get(0), 0.0, 0.0);      // Set position for AS1
    anim.SetConstantPosition(as5.Get(9), 100.0, 100.0); // Set position for AS5

    // Simulate TCP communication between nodes 0 and 20 in AS1 and AS3
    Ptr<Node> sender = as1.Get(0);
    Ptr<Node> receiver = as3.Get(0);

    // Install TCP/IP stack on the nodes
    Ipv4InterfaceContainer interfaces;
    interfaces = address.Assign(devices);

    // Install TCP applications for communication within AS1 and AS3
    uint16_t port = 9; // Port number
    OnOffHelper onoff("ns3::TcpSocketFactory", InetSocketAddress(interfaces.GetAddress(20), port));
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    ApplicationContainer apps = onoff.Install(sender);
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));

    PacketSinkHelper sinkHelper("ns3::TcpSocketFactory", InetSocketAddress(interfaces.GetAddress(20), port));
    apps = sinkHelper.Install(receiver);
    apps.Start(Seconds(0.0));
    apps.Stop(Seconds(10.0));

    Simulator::Stop(Seconds(10)); // Stop the simulation after 3 seconds
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
