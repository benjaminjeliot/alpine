/**
 * This file contains all of the correctness checking code for Kripke.
 */
#include "testKernels.h"

#include <Kripke.h>
#include <Kripke/Grid.h>
#include <Kripke/Input_Variables.h>

/**
 * Functional object to run the LTimes kernel.
 */
struct runLTimes {
  std::string name(void) const { return "LTimes"; }

  void operator ()(Grid_Data *grid_data) const {
    grid_data->kernel->LTimes(grid_data);
  }
};

/**
 * Functional object to run the LPlusTimes kernel.
 */
struct runLPlusTimes {
  std::string name(void) const { return "LPlusTimes"; }

  void operator ()(Grid_Data *grid_data) const {
    grid_data->kernel->LPlusTimes(grid_data);
  }
};


/**
 * Functional object to run the scattering kernel.
 */
struct runScattering {
  std::string name(void) const { return "scattering"; }

  void operator ()(Grid_Data *grid_data) const {
    grid_data->kernel->scattering(grid_data);
  }
};


/**
 * Functional object to run the source kernel.
 */
struct runSource {
  std::string name(void) const { return "source"; }

  void operator ()(Grid_Data *grid_data) const {
    grid_data->kernel->source(grid_data);
  }
};

/**
 * Functional object to run the MPI sweep and sweep kernels
 */
struct runSweep {
  std::string name(void) const { return "Sweep"; }

  void operator ()(Grid_Data *grid_data) const {
    std::vector<int> sdom_list(grid_data->subdomains.size());
    for(int i = 0;i < grid_data->subdomains.size();++ i){
      sdom_list[i] = i;
    }
    SweepSubdomains(sdom_list, grid_data, false);
  }
};


/**
 * Tests a specific kernel (using one of the above runXXX functional objects).
 */
template<typename KernelRunner>
void testKernel(Input_Variables &input_variables){
  int myid;
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);

  KernelRunner kr;

  if(myid == 0){
    printf("  Comparing %s to %s for kernel %s\n",
      nestingString(NEST_GDZ).c_str(),
      nestingString(input_variables.nesting).c_str(),
      kr.name().c_str());
  }

  // Allocate two problems (one reference)
  if(myid == 0)printf("    -- allocating\n");
  Grid_Data *grid_data = new Grid_Data(&input_variables);

  Nesting_Order old_nest = input_variables.nesting;
  input_variables.nesting = NEST_GDZ;
  Grid_Data *ref_data = new Grid_Data(&input_variables);
  input_variables.nesting = old_nest;

  // Generate random data in the reference problem, and copy it to the other
  if(myid == 0)printf("    -- randomizing data\n");
  ref_data->randomizeData();
  grid_data->copy(*ref_data);

  if(myid == 0)printf("    -- running kernels\n");

  // Run both kernels
  kr(ref_data);
  kr(grid_data);

  if(myid == 0)printf("    -- comparing results\n");
  // Compare differences
  bool is_diff = ref_data->compare(*grid_data, 1e-12, true);
  if(is_diff){
    if(myid == 0)printf("Differences found, bailing out\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  // Cleanup
  if(myid == 0)printf("    -- OK\n\n");
  delete grid_data;
  delete ref_data;
}


/**
 * Tests all kernels given the specified input.
 */
void testKernels(Input_Variables &input_variables){
  // Run LTimes
  testKernel<runLTimes>(input_variables);

  // Run LPlusTimes
  testKernel<runLPlusTimes>(input_variables);

  // Run Scattering
  testKernel<runScattering>(input_variables);

  // Run Source
  testKernel<runSource>(input_variables);

  // Run Sweep
  testKernel<runSweep>(input_variables);
}
