//-----------------------------------------------------------------------------
///
/// file: alpine_vtkm_dataset_info.hpp
///
//-----------------------------------------------------------------------------

#ifndef ALPINE_VTKM_DATASET_INFO_HPP
#define ALPINE_VTKM_DATASET_INFO_HPP

#include <string>
#include <map>

#include <vtkm/cont/DataSet.h>
//-----------------------------------------------------------------------------
// -- begin alpine:: --
//-----------------------------------------------------------------------------
namespace alpine
{

class VTKMDataSetInfo
{
public:
 typedef typename vtkm::cont::ArrayHandleUniformPointCoordinates UniformArrayHandle;
 typedef vtkm::cont::ArrayHandle<vtkm::FloatDefault> DefaultHandle;
 typedef typename vtkm::cont::ArrayHandleCartesianProduct<DefaultHandle,
                                                          DefaultHandle,
                                                          DefaultHandle> CartesianArrayHandle;
//-----------------------------------------------------------------------------
  static vtkm::Id GetNumberOfCells(const vtkm::cont::DataSet &data_set)
  {
    const vtkm::cont::DynamicCellSet cell_set = data_set.GetCellSet();
    return GetNumberOfCells(cell_set);
  }

  static vtkm::Id GetNumberOfCells(const vtkm::cont::DynamicCellSet &cell_set)
  {
    return cell_set.GetNumberOfCells();
  }
//-----------------------------------------------------------------------------
  static bool IsExplicit(const vtkm::cont::DataSet &data_set)
  {
    const vtkm::cont::DynamicCellSet cell_set = data_set.GetCellSet();
    return IsExplicit(cell_set);
  }

  static bool IsExplicit(const vtkm::rendering::Actor &actor)
  {
    return IsExplicit(actor.GetCells()); 
  }

  static bool IsExplicit(const vtkm::cont::DynamicCellSet &cell_set)
  {
    bool is_explicit = false;
    if(cell_set.IsSameType(vtkm::cont::CellSetExplicit<>()))
    {
      is_explicit = true;
    }
    else if(cell_set.IsSameType(vtkm::cont::CellSetSingleType<>()))
    {
      is_explicit = true;
    }
    return is_explicit;
  }
//-----------------------------------------------------------------------------
  static bool IsStructured(const vtkm::cont::DataSet &data_set, int &topo_dims)
  {
    const vtkm::cont::DynamicCellSet cell_set = data_set.GetCellSet();
    return IsStructured(cell_set, topo_dims);
  }

  static bool IsStructured(const vtkm::rendering::Actor &actor, int &topo_dims)
  {
    return IsStructured(actor.GetCells(), topo_dims); 
  }

  static bool IsStructured(const vtkm::cont::DynamicCellSet &cell_set, int &topo_dims)
  {
    bool is_structured = false;
    topo_dims = -1;

    if(cell_set.IsSameType(vtkm::cont::CellSetStructured<1>()))
    {
      is_structured = true;
      topo_dims = 1;
    }
    else if(cell_set.IsSameType(vtkm::cont::CellSetStructured<2>()))
    {
      is_structured = true;
      topo_dims = 2;
    }
    else if(cell_set.IsSameType(vtkm::cont::CellSetStructured<3>()))
    {
      is_structured = true;
      topo_dims = 3;
    }
      
    return is_structured;
  }
//-----------------------------------------------------------------------------
  static bool IsRectilinear(const vtkm::cont::DataSet &data_set)
  {
    const vtkm::cont::CoordinateSystem coords = data_set.GetCoordinateSystem();
    return IsRectilinear(coords);
  }

  static bool IsRectilinear(const vtkm::rendering::Actor &actor)
  {
    return IsRectilinear(actor.GetCoordinates()); 
  }

  static bool IsRectilinear(const vtkm::cont::CoordinateSystem &coords)
  {

    bool is_rect= false; 

    if(coords.GetData().IsSameType(CartesianArrayHandle()))
    {
      is_rect = true; 
    }
    return is_rect;
  }
//-----------------------------------------------------------------------------
  static bool IsUniform(const vtkm::cont::DataSet &data_set)
  {
    const vtkm::cont::CoordinateSystem coords = data_set.GetCoordinateSystem();
    return IsUniform(coords);
  }

  static bool IsUniform(const vtkm::rendering::Actor &actor)
  {
    return IsUniform(actor.GetCoordinates()); 
  }

  static bool IsUniform(const vtkm::cont::CoordinateSystem &coords)
  {
    bool is_uniform= false; 
    if(coords.GetData().IsSameType(UniformArrayHandle()))
    {
      is_uniform = true; 
    }
    return is_uniform;
  }

//-----------------------------------------------------------------------------
  static bool GetPointDims(const vtkm::cont::DataSet &data_set, int *dims)
  {
    const vtkm::cont::DynamicCellSet cell_set = data_set.GetCellSet();
    return GetPointDims(cell_set, dims);
  }

  static bool GetPointDims(const vtkm::rendering::Actor &actor, int *dims)
  {
    return GetPointDims(actor.GetCells(), dims); 
  }

  static bool GetPointDims(const vtkm::cont::DynamicCellSet &cell_set, int *dims)
  {
    int topo_dims;
    bool is_structured = IsStructured(cell_set, topo_dims);
    bool success = false;
    if(!is_structured)
    {
      return success;
    }
    else
    {
      success = true;
    }
    
    if(topo_dims == 1)
    {
      vtkm::cont::CellSetStructured<1> cell_set1 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<1>>();  
      vtkm::Id dims1 = cell_set1.GetPointDimensions();
      dims[0] = dims1;
    }
    else if(topo_dims == 2)
    {
      vtkm::cont::CellSetStructured<2> cell_set2 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<2>>();  
      vtkm::Id2 dims2 = cell_set2.GetPointDimensions();
      dims[0] = dims2[0];
      dims[1] = dims2[1];
    }
    else if(topo_dims == 3)
    {
      vtkm::cont::CellSetStructured<3> cell_set3 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<3>>();  
      vtkm::Id3 dims3 = cell_set3.GetPointDimensions();
      dims[0] = dims3[0];
      dims[1] = dims3[1];
      dims[2] = dims3[2];
    }

    return success;

  }

//-----------------------------------------------------------------------------
  static bool GetCellDims(const vtkm::cont::DataSet &data_set, int *dims)
  {
    const vtkm::cont::DynamicCellSet cell_set = data_set.GetCellSet();
    return GetCellDims(cell_set, dims);
  }

  static bool GetCellDims(const vtkm::rendering::Actor &actor, int *dims)
  {
    return GetCellDims(actor.GetCells(), dims); 
  }

  static bool GetCellDims(const vtkm::cont::DynamicCellSet &cell_set, int *dims)
  {
    int topo_dims;
    bool is_structured = IsStructured(cell_set, topo_dims);
    bool success = false;
    if(!is_structured)
    {
      return success;
    }
    else
    {
      success = true;
    }
    
    if(topo_dims == 1)
    {
      vtkm::cont::CellSetStructured<1> cell_set1 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<1>>();  
      vtkm::Id dims1 = cell_set1.GetCellDimensions();
      dims[0] = dims1;
    }
    else if(topo_dims == 2)
    {
      vtkm::cont::CellSetStructured<2> cell_set2 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<2>>();  
      vtkm::Id2 dims2 = cell_set2.GetCellDimensions();
      dims[0] = dims2[0];
      dims[1] = dims2[1];
    }
    else if(topo_dims == 3)
    {
      vtkm::cont::CellSetStructured<3> cell_set3 = 
          cell_set.Cast<vtkm::cont::CellSetStructured<3>>();  
      vtkm::Id3 dims3 = cell_set3.GetCellDimensions();
      dims[0] = dims3[0];
      dims[1] = dims3[1];
      dims[2] = dims3[2];
    }

    return success;

  }
  
};

} 
//-----------------------------------------------------------------------------
// -- end alpine:: --
//-----------------------------------------------------------------------------


#endif
//-----------------------------------------------------------------------------
// -- end header ifdef guard
//-----------------------------------------------------------------------------
