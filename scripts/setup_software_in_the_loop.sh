#!/bin/bash
socat -d -d pty,link=/tmp/vserial_flight_software_side,raw,echo=0 pty,link=/tmp/vserial_sil_side,raw,echo=0
