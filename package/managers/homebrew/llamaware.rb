class Llamaware < Formula
  desc "Professional AI agent with command execution and file operations"
  homepage "https://github.com/harpertoken/llamaware"
  url "https://github.com/harpertoken/llamaware/archive/v1.0.0.tar.gz"
  sha256 "YOUR_SHA256_HERE"
  license "MIT"

  depends_on "cmake" => :build
  depends_on "cpr"
  depends_on "nlohmann-json"

  def install
    system "cmake", "-S", ".", "-B", "build", *std_cmake_args
    system "cmake", "--build", "build"
    bin.install "build/bin/llamaware-agent"
    
    # Install configuration template
    (etc/"llamaware").install ".env.example" => "config.env"
  end

  def post_install
    (var/"llamaware").mkpath
    unless (etc/"llamaware/.env").exist?
      cp etc/"llamaware/config.env", etc/"llamaware/.env"
    end
  end

  test do
    system "#{bin}/llamaware-agent", "--version"
  end

  def caveats
    <<~EOS
      Configuration file is located at:
        #{etc}/llamaware/.env

      Edit this file with your API keys:
        - TOGETHER_API_KEY (for online mode)
        - CEREBRAS_API_KEY (for Cerebras mode)
        - SERPAPI_KEY (for web search)

      Data directory:
        #{var}/llamaware/
    EOS
  end
end