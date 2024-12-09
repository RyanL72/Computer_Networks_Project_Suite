# **Computer Networks Exam Study Guide**

---

## **1. Network Layer Overview**
- **Definition:** Manages data delivery across interconnected networks.
- **Main Functions:**
  - Routing: Determines best paths for data.
  - Forwarding: Transfers packets between interfaces.
  - Addressing: Identifies devices using IP addresses.
  - Encapsulation: Adds headers to data from the transport layer.
  - Fragmentation: Splits large datagrams into smaller packets.

---

## **2. IP Datagram Format**
- **Datagram:** Basic data unit in the IP layer.
- **Structure:**
  - **Header:** Control data for delivery and routing.
  - **Payload:** Actual data being sent.

### **IP Header Fields:**
- **Version:** IP protocol version (IPv4 or IPv6).
- **Header Length:** Size of the IP header in 32-bit words.
- **Type of Service (ToS):** Data handling preferences for quality of service.
- **Total Length:** Size of the entire datagram (header + payload).
- **Identification:** Unique packet identifier for reassembly after fragmentation.
- **Flags:** Control whether fragmentation is allowed.
- **Fragment Offset:** Specifies fragment position in the original datagram.
- **Time to Live (TTL):** Packet lifespan to prevent infinite routing loops.
- **Protocol:** Specifies the transport layer protocol (e.g., TCP, UDP).
- **Header Checksum:** Ensures the integrity of the IP header.
- **Source IP Address:** Original sender's IP address.
- **Destination IP Address:** Receiver's IP address.

---

Let me know if you want additional sections added or more details for any concept!
