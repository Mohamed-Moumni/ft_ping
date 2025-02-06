# ft_ping

A custom implementation of the ping command in C, designed to provide network diagnostics and monitoring capabilities similar to the original ping utility.

## 🚀 Features

- ICMP Echo Request/Reply implementation
- Real-time packet statistics
- RTT (Round Trip Time) calculations
- DNS resolution with reverse lookup
- Configurable packet sizes
- Deadline and count-based execution control
- Checksum calculation and verification
- Statistical analysis including min/avg/max/mdev calculations

## 🛠️ Technical Implementation

### Core Components

- **Raw Socket Programming**: Implements low-level networking using SOCK_RAW
- **ICMP Protocol**: Custom implementation of ICMP header construction and parsing
- **Memory Management**: Careful handling of dynamic memory allocation
- **Signal Handling**: Implements SIGALRM for timing and SIGINT for graceful termination

### Architecture

```
ft_ping/
├── ft_ping.c      # Core ping implementation
├── ft_ping.h      # Header definitions and structures
├── main.c         # Entry point and signal handling
├── parser.c       # Command-line argument parsing
└── utils.c        # Utility functions and calculations
```

## 🔧 Building and Installation

### Prerequisites

- GCC compiler
- Root privileges (required for raw socket operations)
- UNIX-like operating system

### Compilation

```bash
make
```

### Running with Root Privileges

```bash
sudo ./ft_ping [options] <destination>
```

## 🎯 Usage

### Basic Usage

```bash
sudo ./ft_ping google.com
```

### Available Options

- `-h`: Display help information
- `-n`: No DNS name resolution
- `-c <count>`: Stop after sending count packets
- `-w <deadline>`: Specify a timeout in seconds
- `-s <size>`: Use specific packet size
- `-V`: Display version information
- `-v`: Verbose output

## 🔬 Technical Details

### Network Features

- IPv4 support
- ICMP protocol implementation
- DNS resolution with reverse lookup capability
- Real-time RTT calculation
- Statistical analysis of network performance

### Performance Metrics

- Packet loss calculation
- Round-trip time statistics
- Standard deviation calculations
- Minimum/Maximum/Average time tracking

## 🔐 Security Considerations

- Requires root privileges for raw socket operations
- Implements proper input validation
- Handles memory allocation safely
- Validates packet integrity through checksum

## 📊 DevOps Integration

### Monitoring Capabilities

- Real-time packet statistics
- Network latency monitoring
- Packet loss tracking
- Performance metrics collection

### CI/CD Integration

- Can be integrated into network health checks
- Useful for infrastructure monitoring
- Suitable for automated testing pipelines
- Performance baseline establishment

### Observability

- Detailed statistics output
- Real-time performance metrics
- Error handling and logging
- Network diagnostics capabilities

## 🤝 Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📝 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🎉 Acknowledgments

- Inspired by the original ping utility
- Built as part of networking and systems programming education
