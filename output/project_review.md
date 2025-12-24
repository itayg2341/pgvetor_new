# Project Review

## 0) Executive Summary

**pgvector** is a PostgreSQL extension that adds vector similarity search capabilities to PostgreSQL databases. It provides native support for storing, indexing, and querying high-dimensional vector data, enabling efficient similarity searches using various distance metrics.

**Why it exists:** Traditional databases struggle with high-dimensional vector similarity searches, which are fundamental to modern AI/ML applications like recommendation systems, image search, and natural language processing. pgvector brings vector operations directly into PostgreSQL, eliminating the need for separate vector databases.

**High-level architecture:**
- PostgreSQL extension written in C
- Custom data types for vectors (vector, halfvec, bitvec, sparsevec)
- Multiple indexing strategies (IVFFlat, HNSW)
- Distance functions (L2, inner product, cosine, Hamming, Jaccard)
- Seamless integration with PostgreSQL's query planner and executor

**Overall complexity assessment:** HIGH - The codebase involves deep PostgreSQL internals, custom type systems, multiple indexing algorithms, and performance-critical vector operations.

## 1) Repository Map

```
src/                    # Core implementation
├── vector.c           # Main vector type implementation
├── vector.h           # Vector type definitions
├── halfvec.c/h        # Half-precision vector support
├── bitvec.c/h         # Binary vector support
├── sparsevec.c/h      # Sparse vector support
├── ivf*.c/h           # IVFFlat indexing implementation
├── hnsw*.c/h          # HNSW indexing implementation
└── *.h                # Utility headers

sql/                    # SQL interface definitions
├── vector.sql         # Core vector type and functions
├── halfvec.sql        # Half-precision vector functions
├── bitvec.sql         # Binary vector functions
└── sparsevec.sql      # Sparse vector functions

test/                   # Test suite
├── sql/               # SQL test files
└── expected/          # Expected test outputs

Makefile               # Build configuration
META.json              # Extension metadata
```

**Core Logic vs Support Code:**
- **Core Logic:** `src/vector.c`, `src/ivf*.c`, `src/hnsw*.c` - Main vector operations and indexing
- **Support Code:** `src/*utils*.c`, `src/*vec.c` - Utility functions and specialized vector types
- **Entry Points:** SQL functions defined in `sql/*.sql` files, implemented in corresponding C files
- **Lifecycle Boundaries:** PostgreSQL extension lifecycle (load, init, query, cleanup)

## 2) System Architecture

### 2.1 Component Diagram

```
User Application
       ↓
PostgreSQL Client
       ↓
PostgreSQL Server
┌─────────────────────────────────────┐
│         Query Parser                │
├─────────────────────────────────────┤
│       Query Planner                 │
│  ┌─────────────────────────────┐    │
│  │   Vector Cost Estimation    │    │
│  └─────────────────────────────┘    │
├─────────────────────────────────────┤
│       Query Executor                │
│  ┌─────────────────────────────┐    │
│  │   Vector Scan Operations    │    │
│  ├─────────────────────────────┤    │
│  │   Index Scan Operations     │    │
│  │  (IVFFlat/HNSW indexes)     │    │
│  └─────────────────────────────┘    │
├─────────────────────────────────────┤
│     Storage Layer                   │
│  ┌─────────────────────────────┐    │
│  │   Vector Type Storage       │    │
│  ├─────────────────────────────┤    │
│  │   Index Storage             │    │
│  └─────────────────────────────┘    │
└─────────────────────────────────────┘
       ↓
   Disk Storage
```

### 2.2 Data Flow Diagram

```
Vector Data Input
       ↓
Type Conversion (vector_in)
       ↓
Validation & Normalization
       ↓
Storage Allocation
       ↓
┌─────────────────┐
│  TOAST Storage  │ (if large)
└─────────────────┘
       ↓
Page Storage
       ↓
Index Building (optional)
┌─────────────────────────────────┐
│ IVFFlat: Centroid-based clustering
│ HNSW: Hierarchical graph construction
└─────────────────────────────────┘
       ↓
Query Processing
┌─────────────────────────────────┐
│ 1. Parse vector literal         │
│ 2. Choose scan method           │
│ 3. Apply distance function      │
│ 4. Return k nearest neighbors   │
└─────────────────────────────────┘
```

### 2.3 Execution Flow Diagram

```
Extension Load
     ↓
Type Registration
     ↓
Function Registration
     ↓
Operator Registration
     ↓
Index AM Registration
     ↓
Ready for Queries

Query Execution:
SELECT * FROM items
ORDER BY embedding <-> query_vector
LIMIT 10
     ↓
Parse: vector literal parsing
     ↓
Plan: cost estimation, index selection
     ↓
Execute: scan + distance computation
     ↓
Sort: order by distance
     ↓
Limit: return top-k
```

## 3) Execution Walkthroughs

### 3.1 Startup

1. **Extension Loading:** PostgreSQL loads `vector.so` via `CREATE EXTENSION vector`
2. **Type Registration:** Calls `Pg_magic_func` → `Pg_init` → registers custom types
3. **Function Registration:** Registers SQL functions (vector_in, vector_out, etc.)
4. **Operator Registration:** Registers operators (<->, <#>, <=>, etc.)
5. **Index Access Method Registration:** Registers IVFFlat and HNSW index methods
6. **Memory Context Setup:** Initializes long-lived memory contexts for caching

### 3.2 Core Runtime Flow

**Vector Insertion:**
```
INSERT INTO items (embedding) VALUES ('[1,2,3,4,5]')
```
1. `vector_in()` parses string literal → validates format → allocates Vector struct
2. Checks dimension constraints (≤ 16000)
3. Normalizes data (if needed)
4. Stores in TOAST if large, regular storage if small

**Similarity Search:**
```
SELECT * FROM items ORDER BY embedding <-> '[1,2,3,4,5]' LIMIT 10
```
1. Parse query vector using `vector_in()`
2. Query planner estimates costs:
   - Sequential scan: O(n) distance computations
   - Index scan: O(log n) with approximation
3. Choose scan method based on statistics and limits
4. Execute scan:
   - Sequential: compute distance for every row
   - Index: traverse IVFFlat/HNSW structure
5. Sort results by distance
6. Apply LIMIT

### 3.3 Background / Async Processing

**Index Maintenance:**
- **IVFFlat:** Background clustering when building index
- **HNSW:** Incremental graph construction during inserts
- **Vacuum:** Custom vacuum routines for index cleanup

**No separate background workers** - all processing happens in PostgreSQL backend processes during query execution or index operations.

## 4) Key Modules Explained

### vector.c/vector.h
**Purpose:** Core vector type implementation
**Responsibilities:**
- Vector data type definition and memory management
- I/O functions (vector_in, vector_out, vector_send, vector_recv)
- Basic operations (vector construction, dimension validation)
- Type conversion functions

**Dependencies:** PostgreSQL varlena system, TOAST
**Dependents:** All other vector operations, indexes
**Invariants:** Maximum 16000 dimensions, proper varlena formatting

### ivfbuild.c/ivfflat.h
**Purpose:** IVFFlat index implementation
**Responsibilities:**
- Centroid-based clustering of vectors
- Index structure building and maintenance
- K-means clustering algorithm
- List assignment for vectors

**Dependencies:** vector.c, PostgreSQL index AM interface
**Dependents:** Query planner, index scans
**Invariants:** Fixed number of lists, proper centroid updates

### hnsw.c/hnsw.h
**Purpose:** HNSW (Hierarchical Navigable Small World) index
**Responsibilities:**
- Multi-layer graph construction
- Proximity graph maintenance
- Insert operations with graph updates
- Search algorithm implementation

**Dependencies:** vector.c, PostgreSQL index AM interface
**Dependents:** Query planner, index scans
**Invariants:** Graph connectivity, layer assignments

### distance functions
**Purpose:** Various distance metrics
**Responsibilities:**
- L2 (Euclidean) distance: `l2_distance()`
- Inner product: `inner_product()`
- Cosine distance: `cosine_distance()`
- Hamming distance: `hamming_distance()`
- Jaccard distance: `jaccard_distance()`

**Dependencies:** vector.c, specialized vector types
**Dependents:** All index types, sequential scans
**Invariants:** Proper overflow handling, SIMD optimizations

## 5) Configuration & Environment Model

**Build Configuration:**
- `Makefile`: Controls compilation flags, PostgreSQL version detection
- `PG_CONFIG`: Path to pg_config utility
- `USE_AVX2`, `USE_AVX512`: SIMD instruction set flags

**Runtime Configuration:**
- `ivfflat.probes`: Number of lists to probe (default: 1)
- `hnsw.ef_search`: Size of dynamic candidate list (default: 40)
- `maintenance_work_mem`: Memory for index builds
- `max_connections`: Affects concurrent index builds

**Development vs Production:**
- Debug builds include assertions and extra validation
- Release builds optimize for performance (-O3, SIMD)
- Test configurations use smaller dimensions for faster testing

**Secrets Handling:** No secrets required - this is a database extension, not a service.

## 6) Dependency Structure

**External Dependencies:**
- PostgreSQL development headers (version-specific)
- C compiler with C99 support
- Optional: SIMD instruction sets (AVX2, AVX512)

**Internal Dependencies:**
```
vector.c (base type)
    ↑
    ├── halfvec.c (specialized type)
    ├── bitvec.c (specialized type)
    ├── sparsevec.c (specialized type)
    ├── ivf*.c (IVFFlat indexing)
    └── hnsw*.c (HNSW indexing)
```

**Coupling Analysis:**
- **Tight coupling:** Vector type system, index access methods
- **Loose coupling:** Distance functions, utility routines
- **Implicit contracts:** Vector memory layout, index page formats

## 7) Observational Quality & Risk Review

**Maintainability Risks:**
- **PostgreSQL version compatibility:** Tight coupling to PostgreSQL internals
- **C memory management:** Manual memory management risks leaks/corruption
- **Performance-critical code:** Optimizations may obscure correctness
- **Index format compatibility:** On-disk format changes require upgrade paths

**Complexity Hotspots:**
- **HNSW graph algorithms:** Complex graph traversal and maintenance
- **SIMD optimizations:** Platform-specific assembly code
- **PostgreSQL integration:** Deep integration with planner/executor
- **Concurrent index builds:** Coordination between multiple backends

**Testing Challenges:**
- **Nondeterministic results:** Approximate nearest neighbor algorithms
- **Performance regression detection:** Requires large datasets
- **Platform-specific behavior:** SIMD code paths vary by CPU
- **Memory pressure scenarios:** TOAST behavior under memory constraints

**Single Points of Failure:**
- **Vector type corruption:** Could corrupt entire database
- **Index corruption:** Would require index rebuilds
- **Memory leaks:** In long-running backends

## 8) Developer Guide & Onboarding Notes

**Local Development Setup:**
```bash
# Install PostgreSQL development packages
sudo apt-get install postgresql-server-dev-XX

# Build extension
make
make install

# Create test database
createdb test_vector
psql test_vector -c "CREATE EXTENSION vector;"

# Run tests
make installcheck
```

**Codebase Navigation:**
1. Start with `src/vector.c` - understand the basic type system
2. Read `sql/vector.sql` - understand the SQL interface
3. Study `src/ivfflat.h` and `src/hnsw.h` - understand indexing
4. Review `test/sql/` - understand expected behaviors

**Where to Add New Features:**
- **New vector types:** Create new files following pattern of `halfvec.c/h`
- **New distance functions:** Add to `vector.c` or specialized type files
- **New index types:** Create new files following `ivf*.c` or `hnsw*.c` patterns
- **SQL functions:** Add to appropriate `sql/*.sql` file

**Where NOT to Add Logic:**
- Don't modify PostgreSQL core files
- Avoid changing on-disk formats without upgrade paths
- Don't add blocking operations to hot paths

**Common Pitfalls:**
- Forgetting to update both C and SQL files
- Not handling TOAST properly for large vectors
- Missing proper memory context management
- Incorrect PostgreSQL version compatibility checks
- SIMD alignment requirements

## 9) Mental Model & Glossary

**How to Think About This System:**
pgvector extends PostgreSQL with vector capabilities by:
1. Adding new data types that integrate with PostgreSQL's type system
2. Providing indexing methods optimized for vector similarity
3. Implementing distance functions as PostgreSQL operators
4. Leveraging PostgreSQL's existing infrastructure (storage, planner, executor)

Think of it as "PostgreSQL + vectors" rather than a separate vector database.

**Glossary:**
- **Vector:** Array of floating-point numbers representing a point in high-dimensional space
- **IVFFlat:** Inverted File with Flat compression - clustering-based index
- **HNSW:** Hierarchical Navigable Small World - graph-based index
- **TOAST:** PostgreSQL's mechanism for storing large values
- **Distance function:** Mathematical function measuring similarity between vectors
- **varlena:** PostgreSQL's variable-length data type format
- **Index AM:** Index Access Method - PostgreSQL's plugin interface for indexes

**Recommended Reading Order:**
1. `README.md` - Project overview
2. `src/vector.h` - Core data structures
3. `src/vector.c` - Basic operations (first 200 lines)
4. `sql/vector.sql` - SQL interface
5. `src/ivfflat.h` - Index structure
6. `test/sql/vector.sql` - Usage examples
7. Choose specific area: indexing, specialized types, or distance functions

## 10) Appendix

**Entry Points:**
- `src/vector.c:vector_in()` - Vector literal parsing
- `src/vector.c:vector()` - Vector constructor
- `src/ivfbuild.c:ivfflatbuild()` - IVFFlat index creation
- `src/hnswbuild.c:hnswbuild()` - HNSW index creation

**Important File Paths:**
- `src/vector.c` - Main vector implementation
- `src/ivfflat.h` - IVFFlat index definitions
- `src/hnsw.h` - HNSW index definitions
- `sql/vector.sql` - Core SQL interface
- `Makefile` - Build configuration

**Non-obvious Behaviors:**
- Vectors are automatically TOASTed if > 2KB
- Distance functions return NULL for vectors with different dimensions
- Index builds use `maintenance_work_mem` for memory allocation
- IVFFlat requires table to have at least some data before building index
- HNSW inserts are incremental but builds can be batched
