#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/packet-sink.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h" // Include NetAnim
#include "ns3/mobility-module.h" // Include Mobility

using namespace ns3;

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

    // Configure interfaces and assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0"); // Base IP address for ASN 1

    for (uint32_t i = 0; i < devices.GetN(); ++i) {
        address.Assign(devices.Get(i));
    }

    // Create animation object and output the animation to testing.xml
    AnimationInterface anim("testing.xml");
    anim.SetConstantPosition(nodes.Get(0), 0.0, 0.0); // Set position for node 0
    anim.SetConstantPosition(nodes.Get(49), 100.0, 100.0); // Set position for node 49

    // Add Mobility Models (ConstantPositionMobilityModel)
    // MobilityHelper mobility;
    // Ptr<GridPositionAllocator> positionAlloc = CreateObject<GridPositionAllocator>();
    // positionAlloc->SetMinX(0);
    // positionAlloc->SetMinY(0);
    // positionAlloc->SetDeltaX(5);
    // positionAlloc->SetDeltaY(5);
    // positionAlloc->SetLayoutType(GridPositionAllocator::ROW_FIRST);

    // mobility.SetPositionAllocator(positionAlloc);
    // mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    // mobility.Install(nodes);

    // Simulate TCP communication between nodes 0 and 20
    Ptr<Node> sender = nodes.Get(0);
    Ptr<Node> receiver = nodes.Get(20);

    // Install TCP/IP stack on the nodes
    Ipv4InterfaceContainer interfaces;
    interfaces = address.Assign(devices);

    // Install TCP applications
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

    Simulator::Stop(Seconds(10)); // Stop the simulation after 10 seconds
    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
