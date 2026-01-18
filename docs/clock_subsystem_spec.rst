==========================
Clock Subsystem Specification
==========================

Overview
========

This document defines the high-level specification for the clock subsystem
abstraction layer. The design prioritizes compile-time computation and
validation to ensure correctness and zero runtime overhead.

Design Principles
=================

Compile-Time Configuration
--------------------------

All clock configuration shall be computed at compile time. This includes:

* Clock source selection
* PLL parameters (multipliers, dividers)
* Bus prescalers
* Resulting frequencies for all clock domains

The implementation shall use C++23 ``constexpr`` and ``consteval`` constructs
to guarantee compile-time evaluation.

Compile-Time Validation
-----------------------

All configuration constraints shall be validated at compile time. Invalid
configurations shall cause compilation failure with descriptive error messages.

Validation includes:

* Frequency range constraints for each clock domain
* Valid parameter ranges for PLL configuration
* Peripheral clock requirements
* Flash wait state requirements based on target frequency

Runtime Behavior
----------------

At runtime, the clock subsystem shall only apply the pre-computed configuration.
There shall be no runtime reconfiguration capability. The configuration is
immutable once the system starts.

MCU Abstraction
===============

The clock subsystem shall provide a layered architecture supporting both
direct MCU-specific usage and an abstracted interface.

Abstract Interface
------------------

The abstract interface shall provide a simple API with no configuration
parameters, as all configuration is pre-computed at compile time:

* ``init()`` - Initialize the clock subsystem with pre-computed configuration
* ``enable()`` - Enable the configured clocks

Information Retrieval
---------------------

The clock subsystem shall provide a mechanism to retrieve configured clock
frequencies and related information. This enables dependent subsystems
(UART, timers, etc.) to configure their parameters based on actual clock values.

Platform Implementations
------------------------

Each MCU family shall provide implementation files containing:

* All possible clock configurations for that MCU family
* Implementation of all clock features supported by the MCU
* Compile-time constraints and validation rules specific to the MCU

Usage Modes
-----------

The library shall support two usage modes:

Direct Inclusion
^^^^^^^^^^^^^^^^

Users who do not require abstraction may directly include the MCU-specific
header files. This provides:

* Full access to MCU-specific features and configurations
* No abstraction overhead
* Tight coupling to specific MCU family

Abstracted Interface
^^^^^^^^^^^^^^^^^^^^

Users who require portability may link against an object file providing the
abstract interface. This provides:

* MCU-agnostic API
* Implementation selected at compile time based on target board/MCU
* Portability across supported MCU families
