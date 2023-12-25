#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/internet-module.h"
#include "ns3/netanim-module.h" // Include NetAnim

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

    Simulator::Run();
    Simulator::Destroy();

    return 0;
}
