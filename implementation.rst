Implementations
===============

Clock
-----

Specification
^^^^^^^^^^^^^

- Compile time knowledge of clock speed of the various components of the
  system.
- I do not need to change the frequency at runtime, I just configure everything
  once, so i can compute all the parameters at compile time.
- Requests of the clocks which can be decided based on the board?

What is the point of having an abstraction for the clock settings? Using the
framework lets you write a single driver which then can be used by different
boards since you just need to implement the various functions.

I do not need to call these functions from main. I can have a standard main
which linkgs against a power library which computes everything at compile time
in there.
Maybe I can specify a concept that tells that I will have a specific data
structure which holds the info.

Main structure
^^^^^^^^^^^^^^

If we think about coco as something that can be inlcuded in some project and
used as a sort of library which abstracts away the hardware details, the main
is not really decided from coco.

Whenever an application is built using coco, you also have to specify the
configuration to have the correct stuff for coco.

The main will actually be found on the application project which has to be
called by the startup handler defined in coco.

The other tihng we can have is just a library where the application includes
the correct header by itself.

application main includes board header (developer specifies itself the correct
header).

::

   +------------------+      +---------------------+
   | application main | ---> | board-specific      |
   +------------------+      | header              |
                             +---------------------+

application main links against coco library (build configuration selects correct stuff)

::

   +------------------+      +---------------------+      +---------------------+
   | application main | ---> | coco library (.a)   | ---> | board-specific      |
   +------------------+      +---------------------+      | header              |
                                                          +---------------------+

.. code-block:: c++

   clk::configure<Infos>();

   static constexpr usart2_freq {clk:tree::usart_2_freq_hz};

