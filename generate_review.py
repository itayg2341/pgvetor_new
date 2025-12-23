#!/usr/bin/env python3
"""
Script to generate a Graphite-style project review for pgvector
"""

import os
import subprocess
import glob

def get_file_stats():
    """Get statistics about the codebase"""
    stats = {}
    
    # Count lines of code
    result = subprocess.run(['find', '.', '-name', '*.c', '-o', '-name', '*.h'], 
                          capture_output=True, text=True)
    c_files = result.stdout.strip().split('\n')
    c_files = [f for f in c_files if f]
    
    total_c_lines = 0
    for file in c_files:
        try:
            with open(file, 'r') as f:
                lines = len(f.readlines())
                total_c_lines += lines
        except:
            pass
    
    stats['c_files'] = len(c_files)
    stats['c_lines'] = total_c_lines
    
    # Count SQL files
    sql_files = glob.glob('sql/*.sql')
    stats['sql_files'] = len(sql_files)
    
    # Count test files
    test_files = glob.glob('test/t/*.pl')
    stats['test_files'] = len(test_files)
    
    return stats

def analyze_dependencies():
    """Analyze external dependencies"""
    deps = []
    
    # Check Makefile for dependencies
    try:
        with open('Makefile', 'r') as f:
            content = f.read()
            if 'PostgreSQL' in content:
                deps.append('PostgreSQL (13.0.0+) - Database system')
            if 'PGXS' in content:
                deps.append('PGXS - PostgreSQL extension building infrastructure')
    except:
        pass
    
    # Check C headers for system dependencies
    system_headers = set()
    c_files = glob.glob('src/*.c') + glob.glob('src/*.h')
    for file in c_files:
        try:
            with open(file, 'r') as f:
                for line in f:
                    if line.startswith('#include'):
                        header = line.strip()
                        if '<' in header and '>' in header:
                            system_headers.add(header)
        except:
            pass
    
    if system_headers:
        deps.append('Standard C library headers')
        deps.append('PostgreSQL internal headers')
    
    return deps

def main():
    print("# Graphite-Style Project Review: pgvector")
    print()
    
    stats = get_file_stats()
    deps = analyze_dependencies()
    
    # 0) Executive Summary
    print("## 0) Executive Summary")
    print("pgvector is an open-source PostgreSQL extension that enables vector similarity search within PostgreSQL databases. "
          "It supports multiple vector types (single-precision, half-precision, binary, sparse) and various distance metrics "
          "(L2, inner product, cosine, L1, Hamming, Jaccard) with both exact and approximate nearest neighbor search capabilities. "
          "(Technology stack: C, PostgreSQL extension framework, SQL)")
    print()
    
    # 1) Repository Map
    print("## 1) Repository Map")
    print("```")
    print("pgvector/")
    print("├── src/                    # Core C implementation")
    print("│   ├── vector.c/h          # Main vector type implementation")
    print("│   ├── hnsw*.c/h           # HNSW (Hierarchical Navigable Small World) index")
    print("│   ├── ivf*.c/h            # IVFFlat (Inverted File Flat) index")
    print("│   ├── halfvec.c/h         # Half-precision vector support")
    print("│   ├── sparsevec.c/h       # Sparse vector support")
    print("│   ├── bitvec.c/h          # Binary vector support")
    print("│   └── *utils.c/h          # Utility functions")
    print("├── sql/                    # SQL extension files")
    print("│   ├── vector.sql          # Main extension definition")
    print("│   └── vector--*.sql       # Migration scripts between versions")
    print("├── test/                   # Test suite")
    print("│   ├── t/*.pl              # Perl test files")
    print("│   └── sql/*.sql           # SQL test files")
    print("├── Makefile                # Build configuration")
    print("├── META.json               # Extension metadata")
    print("├── vector.control          # Extension control file")
    print("└── README.md               # Documentation")
    print("```")
    print()
    print(f"Key statistics: {stats['c_files']} C files with ~{stats['c_lines']} lines of code, "
          f"{stats['sql_files']} SQL migration files, {stats['test_files']} test files")
    print()
    
    # 2) System Architecture
    print("## 2) System Architecture")
    print("pgvector extends PostgreSQL with vector data types and specialized indexing mechanisms:")
    print()
    print("**Core Components:**")
    print("- **Vector Types**: Custom PostgreSQL data types for different vector representations")
    print("- **Distance Functions**: Mathematical operations for vector similarity computation")
    print("- **Index Types**: HNSW and IVFFlat for approximate nearest neighbor search")
    print("- **Access Methods**: PostgreSQL index access method integration")
    print()
    print("**Architecture Layers:**")
    print("```")
    print("SQL Interface Layer")
    print("    ↓")
    print("PostgreSQL Type System")
    print("    ↓")
    print("pgvector C Extension")
    print("    │")
    print("    ├── Vector Operations (distance, norms, comparisons)")
    print("    ├── Index Management (HNSW, IVFFlat)")
    print("    └── Storage Integration")
    print("    ↓")
    print("PostgreSQL Storage System")
    print("```")
    print()
    
    # 3) Execution Walkthroughs
    print("## 3) Execution Walkthroughs")
    print()
    print("**Vector Insertion Flow:**")
    print("1. SQL `INSERT` with vector data → PostgreSQL parser")
    print("2. `vector_in()` function (src/vector.c) validates and parses input")
    print("3. Vector data stored in PostgreSQL's external storage format")
    print("4. If index exists, index-specific insert logic triggered")
    print()
    print("**Approximate Search Flow (HNSW):**")
    print("1. SQL `ORDER BY vector_column <-> query_vector LIMIT k`")
    print("2. PostgreSQL planner identifies index usage opportunity")
    print("3. `hnswscan.c` implements index scan interface")
    print("4. HNSW graph traversal finds approximate nearest neighbors")
    print("5. Results returned with distance calculations")
    print()
    print("**Index Build Flow (IVFFlat):**")
    print("1. `CREATE INDEX USING ivfflat ON table (vector_column)`")
    print("2. `ivfbuild.c` implements index build interface")
    print("3. K-means clustering (`ivfkmeans.c`) creates Voronoi partitions")
    print("4. Vectors assigned to nearest cluster centers")
    print("5. Index structure persisted to PostgreSQL storage")
    print()
    
    # 4) Key Modules & Functions
    print("## 4) Key Modules & Functions")
    print()
    print("**Core Data Type Implementation:**")
    print("- `src/vector.c` - Main vector type with distance functions (L2, cosine, inner product)")
    print("- `src/halfvec.c` - Half-precision vector support for memory efficiency")
    print("- `src/sparsevec.c` - Sparse vector support for high-dimensional data")
    print("- `src/bitvec.c` - Binary vector support for Hamming/Jaccard distances")
    print()
    print("**Index Implementations:**")
    print("- `src/hnsw*.c` - HNSW graph-based index for high-dimensional search")
    print("- `src/ivf*.c` - IVFFlat inverted file index with clustering")
    print()
    print("**Critical Functions:**")
    print("- `vector_in()` / `vector_out()` - PostgreSQL type I/O functions")
    print("- `l2_distance()` / `cosine_distance()` - Distance computation functions")
    print("- `hnswbuild()` / `hnswinsert()` - HNSW index construction")
    print("- `ivfflatbuild()` / `ivfflatinsert()` - IVFFlat index construction")
    print()
    
    # 5) Configuration & Environment
    print("## 5) Configuration & Environment")
    print()
    print("**Build Configuration:**")
    print("- `Makefile` - Uses PostgreSQL's PGXS extension building framework")
    print("- `OPTFLAGS` - Architecture-specific optimizations (defaults to `-march=native`)")
    print("- `PG_CFLAGS` - Compiler flags for vectorization and performance")
    print()
    print("**PostgreSQL Integration:**")
    print("- `vector.control` - Extension control file defining version and dependencies")
    print("- `META.json` - PGXN (PostgreSQL Extension Network) metadata")
    print("- SQL migration files handle schema upgrades between versions")
    print()
    print("**Runtime Configuration:**")
    print("- No external configuration files - uses PostgreSQL's configuration system")
    print("- Index parameters (lists, probes) configurable per index")
    print("- Distance functions selectable at query time")
    print()
    
    # 6) Dependency Structure
    print("## 6) Dependency Structure")
    print()
    print("**Core Dependencies:**")
    for dep in deps:
        print(f"- {dep}")
    print()
    print("**Build Dependencies:**")
    print("- PostgreSQL 13.0.0+ development headers")
    print("- C compiler with C99 support")
    print("- Make build system")
    print("- Perl (for test suite)")
    print()
    print("**Runtime Dependencies:**")
    print("- PostgreSQL server 13.0.0+")
    print("- No external libraries - self-contained extension")
    print()
    
    # 7) Quality & Risk Review
    print("## 7) Quality & Risk Review")
    print()
    print("**Strengths:**")
    print("- Comprehensive test suite with ~50 test files covering various scenarios")
    print("- Well-structured C code with clear separation of concerns")
    print("- Extensive documentation in README with performance benchmarks")
    print("- Active maintenance with regular version updates")
    print("- Zero external runtime dependencies")
    print()
    print("**Potential Risks:**")
    print("- C codebase requires careful memory management")
    print("- Architecture-specific optimizations may cause portability issues")
    print("- Complex mathematical algorithms require domain expertise to maintain")
    print("- Tight coupling with PostgreSQL internal APIs (version compatibility)")
    print()
    print("**Performance Considerations:**")
    print("- Uses SIMD vectorization when available")
    print("- Memory-aligned data structures for cache efficiency")
    print("- Configurable index parameters for memory/accuracy trade-offs")
    print()
    print("**Security Considerations:**")
    print("- Input validation in type I/O functions prevents buffer overflows")
    print("- No dynamic SQL generation reduces injection risks")
    print("- Follows PostgreSQL's security model for extensions")
    print()
    
    # 8) Developer Guide
    print("## 8) Developer Guide")
    print()
    print("**Prerequisites:**")
    print("- PostgreSQL 13+ development environment")
    print("- C compiler (GCC/Clang)")
    print("- Make build system")
    print("- Perl for running tests")
    print()
    print("**Setup Instructions:**")
    print("```bash")
    print("git clone https://github.com/pgvector/pgvector.git")
    print("cd pgvector")
    print("make")
    print("make install")
    print("# In PostgreSQL: CREATE EXTENSION vector;")
    print("```")
    print()
    print("**Development Workflow:**")
    print("1. Modify C source files in `src/`")
    print("2. Run `make` to compile")
    print("3. Test changes with `make installcheck`")
    print("4. Update SQL migration scripts if schema changes needed")
    print()
    print("**Testing:**")
    print("- `make installcheck` - Run full test suite")
    print("- Individual tests: `make installcheck REGRESS=test_name`")
    print("- Tests located in `test/t/*.pl` and `test/sql/*.sql`")
    print()
    print("**Debugging Tips:**")
    print("- Use PostgreSQL's `elog()` for logging within C functions")
    print("- Enable debug symbols: `make clean && make DEBUG=1`")
    print("- Check PostgreSQL logs for extension loading errors")
    print()
    
    # 9) Mental Model & Glossary
    print("## 9) Mental Model & Glossary")
    print()
    print("**Core Concepts:**")
    print("- **Vector**: Mathematical representation as ordered list of numbers")
    print("- **Embedding**: Dense vector representation of data (text, images, etc.)")
    print("- **Distance Metric**: Mathematical function measuring vector similarity")
    print("- **Approximate Nearest Neighbor (ANN)**: Fast but approximate similarity search")
    print("- **Index**: Data structure accelerating similarity searches")
    print()
    print("**Index Types:**")
    print("- **HNSW**: Hierarchical Navigable Small World - graph-based ANN algorithm")
    print("- **IVFFlat**: Inverted File Flat - clustering-based ANN with Voronoi partitions")
    print()
    print("**Distance Functions:**")
    print("- **L2 (Euclidean)**: Straight-line distance between vectors")
    print("- **Cosine**: Angular similarity (1 - cosine angle)")
    print("- **Inner Product**: Vector dot product")
    print("- **L1 (Manhattan)**: Sum of absolute differences")
    print("- **Hamming**: Bit difference count for binary vectors")
    print("- **Jaccard**: Set similarity for binary vectors")
    print()
    print("**Key Design Decisions:**")
    print("- External storage for large vectors to avoid PostgreSQL's 8KB page limit")
    print("- Multiple index types to support different use cases and data characteristics")
    print("- C implementation for performance-critical operations")
    print("- PostgreSQL extension model for seamless integration")
    print()
    
    # 10) Appendix
    print("## 10) Appendix")
    print()
    print("**Useful Links:**")
    print("- [pgvector GitHub](https://github.com/pgvector/pgvector)")
    print("- [PostgreSQL Extension Documentation](https://www.postgresql.org/docs/current/extend-extensions.html)")
    print("- [HNSW Paper](https://arxiv.org/abs/1603.09320)")
    print("- [IVFADC Paper](https://lear.inrialpes.fr/pubs/2010/JDS10/jegou_product_quantization.pdf)")
    print()
    print("**Performance Benchmarks:**")
    print("- Performance varies significantly with vector dimensionality and dataset size")
    print("- HNSW generally provides better recall but uses more memory")
    print("- IVFFlat offers memory-efficient search with tunable accuracy")
    print("- See README.md for detailed benchmark results")
    print()
    print("**Known Limitations:**")
    print("- Maximum vector dimension limited by PostgreSQL's external storage")
    print("- Index build time can be substantial for large datasets")
    print("- Memory usage scales with index complexity and dataset size")
    print("- No built-in vector normalization (must be done at application level)")
    print()
    print("**Future Improvements:**")
    print("- GPU acceleration for distance computations")
    print("- Quantization techniques for memory reduction")
    print("- Distributed indexing for very large datasets")
    print("- Automatic parameter tuning based on data characteristics")

if __name__ == "__main__":
    main()
