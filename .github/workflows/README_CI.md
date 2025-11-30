# IoT Environment Monitor - CI Status

![CI Status](https://github.com/yourusername/IoT_EnvMonitorSys_Basic/workflows/CI/badge.svg)

## Build Status

| Component | Status |
|-----------|--------|
| C Firmware | ✅ Built on push |
| Python Services | ✅ Tested on push |
| Integration Tests | ✅ Run on PR |
| Code Quality | ✅ Validated |

## CI Pipeline

This project uses GitHub Actions for continuous integration:

1. **C Firmware Build** - Compiles the embedded firmware
2. **Python Service Tests** - Validates data collection services  
3. **Integration Tests** - End-to-end workflow validation
4. **Code Quality** - Structure and configuration checks
5. **Security Scan** - Basic security validation

## Local Development

To run the same checks locally:

```bash
# Build firmware
cd IoT_EnvMonitorSys_Basic/firmware
mkdir build && cd build
cmake .. && cmake --build .

# Test Python services
cd ../cloud-services/data-collector
pip install -r requirements.txt
python -m py_compile simple_collector.py

# Run tests
cd ../../../test-engine
pytest tests/ -v