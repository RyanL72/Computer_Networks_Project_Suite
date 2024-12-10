# **Computer Networks Exam Study Guide**

---

## **1. Network Layer Overview**
- **Definition:** Manages data delivery across interconnected networks.
- **Main Functions:**
  - `Routing`: Determines best paths for data.
  - `Forwarding`: Transfers packets between interfaces.
  - `Addressing`: Identifies devices using IP addresses.
  - `Encapsulation`: Adds IP headers to TCP packet making it a [datagram](#2-ip-datagram-format).
  - [`Fragmentation`](#3-fragmentation): Splits large [datagrams](#2-ip-datagram-format) into smaller packets.

---

## **2. IP Datagram Format**
- **`Datagram`:** Basic data unit in the IP layer.
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

---

## **3. Fragmentation**
- **Definition:** The process of breaking down large IP datagrams into smaller fragments to fit the **Maximum Transmission Unit (MTU)** of a network link.
  
### **Why Fragmentation Happens:**
- **MTU Limitations:** Different networks have different MTU sizes. If a datagram is larger than the MTU of a network link, it must be fragmented.
- **Example:** Ethernet’s MTU is typically 1500 bytes.

### **Fragmentation Process:**
1. **Datagram Splitting:** 
   - The original datagram is split into smaller fragments, each with its own IP header.
   
2. **Header Updates:** 
   - Each fragment gets its own IP header with updated fields:
     - **Identification:** Remains the same for all fragments of the original datagram.
     - **Flags:** 
       - **DF (Don't Fragment):** Prevents fragmentation if set.
       - **MF (More Fragments):** Set for all fragments except the last one.
     - **Fragment Offset:** Indicates the position of the fragment relative to the original datagram’s payload.

3. **Reassembly:** 
   - Happens **only at the destination host**, using the `Identification`, `Fragment Offset`, and `MF` flags to reconstruct the original datagram.

---

### **Key Fragmentation Fields in IP Header:**
- **Identification:** Stays the same for all fragments.
- **Flags:** Controls fragmentation behavior.
- **Fragment Offset:** Specifies fragment position in 8-byte units.

---

### **Example Fragmentation Calculation:**
**Given:** 
- Datagram total length: 4000 bytes
- IP header length: 20 bytes
- MTU of next link: 1500 bytes

**Process:**
1. Maximum payload size per fragment = MTU - Header Length = 1500 - 20 = **1480 bytes**.
2. Number of fragments needed = `ceil(4000 - 20) / 1480 = ceil(3980 / 1480) = 3 fragments`
3. Fragment breakdown:
   - Fragment 1: Data size 1480 bytes, Offset 0, MF = 1
   - Fragment 2: Data size 1480 bytes, Offset 185 (1480/8), MF = 1
   - Fragment 3: Data size 1020 bytes, Offset 370, MF = 0 (last fragment)

---

### **Important Notes:**
- Fragmentation reduces efficiency due to multiple headers.
- Avoiding fragmentation is preferable through techniques like **Path MTU Discovery (PMTUD)**.

---


Make sure to know how to calculate payloads
