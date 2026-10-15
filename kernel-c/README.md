# Aether C Kernel — architecture decision

## Goal

Develop an independent x86-64 Aether kernel in C, with a small amount of assembly for CPU entry/exit and other architecture-specific operations. This directory is a separate kernel-development track; it does not replace the current Linux-based ISO build.

## Language decision

- **C11 (freestanding)** for core kernel and native drivers. C has a stable low-level ABI and is the practical foundation for implementing Windows-like kernel interfaces.
- **x86-64 assembly** only where required for boot entry, interrupt stubs, context switching, and CPU control instructions.
- **C++** may be introduced for selected subsystems later, but it is not required for the initial kernel.
- Build as a freestanding target; do not depend on a host C runtime or standard library at kernel runtime.

## Compatibility reality

Using C does not itself make Windows drivers binary-compatible. Windows kernel drivers depend on Windows-specific APIs, object semantics, I/O request handling, Plug and Play, power management, DMA, interrupt behavior, and often undocumented details. Compatibility must be implemented and tested interface-by-interface. No claim of universal `.sys` support is made.

Windows applications are a separate workstream: PE/COFF loading and Win32 API compatibility should be developed separately from kernel-mode driver compatibility.

## Milestones

1. **Bootable baseline:** x86-64 entry, serial diagnostics, halt loop, reproducible cross-build.
2. **CPU foundation:** GDT/IDT, exceptions, interrupt controllers, timer, physical and virtual memory.
3. **Execution model:** scheduler, threads, process address spaces, system-call boundary and user mode.
4. **Device model:** PCI discovery, resource ownership, DMA and native driver interfaces.
5. **Userland:** executable loader, system services, filesystem and test harness.
6. **Windows application compatibility:** PE loader and Win32 APIs, starting with small console programs.
7. **Windows driver compatibility:** selected WDM interfaces first; KMDF and broader support only after foundational semantics and tests exist.

## Engineering rules

- Keep this kernel separate from the existing Linux ISO pipeline until it has its own explicit build workflow.
- Build and test each milestone before adding the next.
- Use QEMU for repeatable smoke tests, then test on real hardware.
- Never describe an artifact as bootable until the produced image and its boot test have been verified.
- Prefer documented interfaces and open-source implementations as references; preserve license obligations when reusing code.
