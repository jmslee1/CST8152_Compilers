\# CST8152 Compilers - Assignment 2: Reader Implementation


\*\*Student:\*\* James  

\*\*Language Name:\*\* J  

\*\*Language Type:\*\* TCL-like Domain Specific Language (DSL)

\---

\## Language Proposal

\*\*J\*\* is a lightweight, command-based scripting language designed for configuring and controlling embedded systems and IoT devices, particularly drone tracking and recovery systems.

It follows a simple TCL-like syntax that is easy to read and write. The goal of J is to allow engineers and technicians to define device behavior (such as recovery logic, thresholds, and communication settings) using human-readable scripts instead of hard-coding everything in firmware.

\### Example J Syntax

```j

device set voltage 1250

if {voltage > 1000} {

&#x20;   enable high\_voltage\_mode

&#x20;   log "Warning: High voltage"

}

run recovery\_sequence

