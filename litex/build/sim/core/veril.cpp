/* Copyright (C) 2017 LambdaConcept */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Vsim.h"
#include "verilated.h"
#ifdef TRACE_FST
#include "verilated_fst_c.h"
#else
#include "verilated_vcd_c.h"
#endif

#ifdef TRACE_FST
VerilatedFstC* tfp;
#else
VerilatedVcdC* tfp;
#endif
uint64_t tfp_start;
uint64_t tfp_end;
uint64_t main_time = 0;

extern "C" void litex_sim_eval(void *vsim)
{
  Vsim *sim = (Vsim*)vsim;
  sim->eval();
}

extern "C" void litex_sim_increment_time(unsigned long dt_ps) {
    main_time += dt_ps;
}

extern "C" void litex_sim_init_cmdargs(int argc, char *argv[])
{
  Verilated::commandArgs(argc, argv);
}

extern "C" void litex_sim_init_tracer(void *vsim, long start, long end)
{
  Vsim *sim = (Vsim*)vsim;
  tfp_start = start;
  tfp_end = end >= 0 ? end : UINT64_MAX;
  Verilated::traceEverOn(true);
#ifdef TRACE_FST
      tfp = new VerilatedFstC;
      sim->trace(tfp, 99);
      tfp->open("sim.fst");
#else
      tfp = new VerilatedVcdC;
      sim->trace(tfp, 99);
      tfp->open("sim.vcd");
#endif
  tfp->set_time_unit("1ps");
  tfp->set_time_resolution("1ps");
}

extern "C" void litex_sim_tracer_dump()
{
  if (tfp_start <= main_time && main_time <= tfp_end) {
    tfp->dump(main_time);
  }
}

extern "C" int litex_sim_got_finish()
{
  return Verilated::gotFinish();
}

#if VM_COVERAGE
extern "C" void litex_sim_coverage_dump()
{
  VerilatedCov::write("sim.cov");
}
#endif

double sc_time_stamp()
{
  return main_time;
}
