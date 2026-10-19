\# C2000 Non-Blocking FSM \& Hardware Interrupts



A clean-architecture, interrupt-driven Finite State Machine (FSM) implemented on the Texas Instruments C2000 LaunchPad using DriverLib.



\## Features

\- \*\*Deterministic Timing:\*\* Uses C2000 CPU Timer 0 hardware interrupts to trigger state transitions precisely every 500ms, eliminating timing drift.

\- \*\*Non-Blocking Architecture:\*\* Keeps the `while(1)` loop free to handle background tasks without blocking delays.

\- \*\*Edge-Detection Button Logic:\*\* Safely reads active-low inputs using pull-up configurations and state-tracking edge detection to toggle system states.



\## Tech Stack

\- \*\*Hardware:\*\* TI C2000 LaunchPad (F28379D)

\- \*\*Framework:\*\* TI DriverLib

\- \*\*Language:\*\* C

