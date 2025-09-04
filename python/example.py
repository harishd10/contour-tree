from __future__ import annotations
import argparse
import time
from pathlib import Path

import numpy as np

try:
    import pyct as ct
except ImportError as e:
    raise SystemExit(
        "Failed to import 'pyct' Python extension. Build the module via CMake first."
    ) from e


def example_processing(data_name: str, dimx: int, dimy: int, dimz: int, persistence: bool = True) -> None:
    """
    Replicates exampleProcessing<T> from C++ main.cpp using the Python bindings.
    Expects data at f"{data_name}.raw" containing dimx*dimy*dimz uint8 values (row-major x,y,z).
    Produces contour tree outputs (.rg.dat/.rg.bin/.part.raw) and simplification order (.order.dat/.order.bin).
    """
    data_path = Path(f"{data_name}.raw")
    if not data_path.exists():
        raise FileNotFoundError(f"Input file not found: {data_path}")

    # Load grid into the provided Grid3DUint8
    grid = ct.Grid3DUint8(dimx, dimy, dimz)
    t0 = time.time()
    grid.loadGrid(str(data_path))

    # Compute contour tree
    tree = ct.MergeTree()
    tree_type = ct.TreeType.TypeContourTree
    print("computing join tree")
    tree.computeTree(grid, tree_type)
    t1 = time.time()
    print(f"Time to compute contour tree: {(t1 - t0)*1000:.0f}ms")
    tree.output(data_name, tree_type)

    # Build hierarchical segmentation (simplification)
    print("creating hierarchical segmentation")
    t2 = time.time()
    ctdata = ct.ContourTreeData()
    ctdata.loadBinFile(data_name)

    sim = ct.SimplifyCT()
    sim.setInput(ctdata)

    if persistence:
        sim_fn = ct.Persistence(ctdata)
    else:
        part_file = f"{data_name}.part.raw"
        sim_fn = ct.HyperVolume(ctdata, part_file)

    sim.simplify(sim_fn)
    t3 = time.time()
    print(f"Time to simplify: {(t3 - t2)*1000:.0f}ms")

    sim.outputOrder(data_name)
    print("done")


def example_querying(data_name: str, use_arcs: bool, topk: int = -1, threshold: float = 0.0):
    """Replicates exampleQuerying from C++ main.cpp and returns a list of Feature."""
    topo = ct.TopologicalFeatures()
    topo.loadData(data_name)

    if use_arcs:
        # extremum-saddle pairs; partition by leaf arcs
        features = topo.getArcFeatures(topk, threshold)
    else:
        # partition by the branch decomposition
        features = topo.getPartitionedExtremaFeatures(topk, threshold)

    return features


def main():
    p = argparse.ArgumentParser(description="Contour Tree Python demo")
    p.add_argument("data_name", help="Path prefix to the input grid (without extension)")
    p.add_argument("dimx", type=int, help="Grid dimension X")
    p.add_argument("dimy", type=int, help="Grid dimension Y")
    p.add_argument("dimz", type=int, help="Grid dimension Z")
    p.add_argument("--compute", action="store_true", help="Run example_processing")
    p.add_argument("--no-persistence", dest="persistence", action="store_false", help="Use HyperVolume instead of Persistence for simplification")
    p.add_argument("--query", action="store_true", help="Run the example querying and print number of features")
    p.add_argument("--topk", type=int, default=-1, help="Top-k features for querying (use -1 to disable)")
    p.add_argument("--threshold", type=float, default=0.0, help="Simplification Threshold for querying (used when topk = -1)")

    args = p.parse_args()

    if args.compute:
        example_processing(args.data_name, args.dimx, args.dimy, args.dimz, persistence=args.persistence)

    if args.query:
        print("querying")
        feats = example_querying(args.data_name, use_arcs=True, topk=args.topk, threshold=args.threshold)
        print(f"Arc features: {len(feats)}")
        feats2 = example_querying(args.data_name, use_arcs=False, topk=args.topk, threshold=args.threshold)
        print(f"Partitioned extrema features: {len(feats2)}")


if __name__ == "__main__":
    main()
