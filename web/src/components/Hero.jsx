import React, { useState, useEffect, useMemo } from 'react';
import { motion } from 'framer-motion';

const Hero = () => {
  const [displayText, setDisplayText] = useState('Llamaware');
  const [isAnimating, setIsAnimating] = useState(false);
  const words = useMemo(() => ['Llamaware', 'Ingenuity'], []);
  const [currentWordIndex, setCurrentWordIndex] = useState(0);

  const scrambleText = (targetText, duration = 1500) => {
    const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
    const targetLength = targetText.length;
    let currentIteration = 0;
    const totalIterations = duration / 50; // 50ms per iteration
    
    setIsAnimating(true);
    
    const interval = setInterval(() => {
      const progress = currentIteration / totalIterations;
      let scrambledText = '';
      
      for (let i = 0; i < targetLength; i++) {
        if (progress > i / targetLength) {
          // This character has "settled"
          scrambledText += targetText[i];
        } else {
          // This character is still scrambling
          scrambledText += chars[Math.floor(Math.random() * chars.length)];
        }
      }
      
      setDisplayText(scrambledText);
      currentIteration++;
      
      if (currentIteration >= totalIterations) {
        clearInterval(interval);
        setDisplayText(targetText);
        setIsAnimating(false);
      }
    }, 50);
  };

  useEffect(() => {
    const mainInterval = setInterval(() => {
      const nextIndex = (currentWordIndex + 1) % words.length;
      const nextWord = words[nextIndex];
      setCurrentWordIndex(nextIndex);
      scrambleText(nextWord);
    }, 4000); // Change every 4 seconds (longer to see the effect)

    return () => clearInterval(mainInterval);
  }, [currentWordIndex, words]);
  return (
    <section className="hero">
      <div className="container">
        <motion.div 
          className="hero-content"
          initial={{ opacity: 0, y: 50 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 0.8 }}
        >
          <motion.div 
            className="hero-logo"
            initial={{ opacity: 0, y: 20 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.6, delay: 0.2 }}
          >
            <div className="logo-container">
              <svg viewBox="0 0 100 100" className="logo-svg" xmlns="http://www.w3.org/2000/svg">
                <rect x="22" y="22" width="12" height="20" fill="#609966" />
                <rect x="44" y="22" width="12" height="20" fill="#609966" />
                <rect x="66" y="22" width="12" height="20" fill="#609966" />
                <rect x="33" y="42" width="12" height="28" fill="#609966" />
                <rect x="55" y="42" width="12" height="28" fill="#609966" />
              </svg>
              <div className={`logo-text ${isAnimating ? 'animating' : ''}`}>
                {displayText}
              </div>
            </div>
          </motion.div>

          <motion.h1 
            className="hero-title"
            initial={{ opacity: 0, y: 30 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.8, delay: 0.4 }}
          >
            AI Agent for Developers
          </motion.h1>

          <motion.div 
            className="hero-visual"
            initial={{ opacity: 0, y: 30 }}
            animate={{ opacity: 1, y: 0 }}
            transition={{ duration: 0.8, delay: 0.6 }}
          >
            <div className="tablet-device">
              <div className="tablet-screen">
                <div className="screen-content">
                  <div className="terminal-header">
                    <span className="terminal-title">llamaware enterprise agent</span>
                    <div className="mode-indicator">
                      <span>cross-platform mode</span>
                    </div>
                  </div>
                  <div className="terminal-content">
                    <div className="command-line">
                      <span className="prompt">llamaware v2.1.0 - enterprise platform ready</span>
                    </div>
                    <div className="status-line">
                      <span className="status">16 enterprise features | linux | macos | windows</span>
                    </div>
                    <div className="command-line">
                      <span className="prompt">›</span>
                      <span className="input">/features</span>
                    </div>
                    <div className="help-output">
                      <div className="output-line">=== enterprise features ===</div>
                      <div className="output-line">• file injection (@file, @directory)</div>
                      <div className="output-line">• session management (save/resume)</div>
                      <div className="output-line">• mcp server support</div>
                      <div className="output-line">• docker sandboxing</div>
                      <div className="output-line">• theme system (/theme)</div>
                      <div className="output-line">• checkpointing (/restore)</div>
                    </div>
                    <div className="command-line">
                      <span className="prompt">›</span>
                      <span className="input">/sandbox docker run python:3.11</span>
                    </div>
                    <div className="command-output">
                      <div className="output-line">[sandboxing]: creating secure container</div>
                      <div className="output-line">container: llamaware_sandbox_001</div>
                      <div className="output-line">status: ready for safe execution</div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </motion.div>
        </motion.div>
      </div>

      <style jsx>{`
        .hero {
          min-height: 100vh;
          display: flex;
          align-items: center;
          position: relative;
          background: #ffffff;
          text-align: center;
          padding: 120px 0;
        }

        .hero-content {
          max-width: 1000px;
          margin: 0 auto;
        }

        .hero-logo {
          margin-bottom: 60px;
        }

        .logo-container {
          display: flex;
          flex-direction: column;
          align-items: center;
          gap: 16px;
        }

        .logo-svg {
          width: 60px;
          height: 60px;
          filter: drop-shadow(0 4px 8px rgba(96, 153, 102, 0.2));
        }

        .logo-text {
          font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
          font-size: 2rem;
          font-weight: 300;
          color: #1a202c;
          letter-spacing: -0.02em;
          transition: all 0.2s ease-in-out;
          min-height: 2.5rem;
          display: flex;
          align-items: center;
          justify-content: center;
        }

        .logo-text.animating {
          font-family: 'JetBrains Mono', 'Courier New', monospace;
          color: #609966;
          letter-spacing: 0.1em;
          text-shadow: 0 0 8px rgba(96, 153, 102, 0.3);
        }

        .hero-title {
          font-family: 'Helvetica Neue', Helvetica, Arial, sans-serif;
          font-size: 4.5rem;
          font-weight: bold;
          color: #1a202c;
          margin-bottom: 80px;
          line-height: 1.05;
          letter-spacing: -0.025em;
        }

        .hero-visual {
          margin-bottom: 60px;
          display: flex;
          justify-content: center;
        }

        .tablet-device {
          width: 400px;
          height: 280px;
          background: #000000;
          border-radius: 20px;
          padding: 8px;
          box-shadow: 0 20px 40px rgba(0, 0, 0, 0.1);
          transform: rotate(-5deg);
        }

        .tablet-screen {
          width: 100%;
          height: 100%;
          background: #1a202c;
          border-radius: 12px;
          padding: 20px;
          position: relative;
          overflow: hidden;
        }

        .screen-content {
          height: 100%;
          display: flex;
          flex-direction: column;
        }

        .terminal-header {
          display: flex;
          justify-content: space-between;
          align-items: center;
          margin-bottom: 20px;
        }

        .terminal-title {
          font-size: 0.8rem;
          color: rgba(255, 255, 255, 0.8);
          font-weight: 500;
        }

        .mode-indicator {
          background: rgba(56, 161, 105, 0.2);
          padding: 4px 8px;
          border-radius: 12px;
          font-size: 0.6rem;
          color: #38a169;
          font-weight: 500;
        }

        .terminal-content {
          flex: 1;
          display: flex;
          flex-direction: column;
          gap: 8px;
          font-family: 'JetBrains Mono', monospace;
          font-size: 0.7rem;
          line-height: 1.4;
        }

        .command-line {
          display: flex;
          align-items: center;
          gap: 8px;
        }

        .prompt {
          color: #38a169;
          font-weight: 600;
        }

        .input {
          color: rgba(255, 255, 255, 0.9);
          font-weight: 400;
        }

        .status-line {
          color: #38a169;
          font-weight: 500;
          margin-bottom: 8px;
        }

        .help-output {
          display: flex;
          flex-direction: column;
          gap: 2px;
          margin-bottom: 8px;
        }

        .command-output {
          display: flex;
          flex-direction: column;
          gap: 2px;
        }

        .output-line {
          color: rgba(255, 255, 255, 0.8);
          font-weight: 400;
        }

        @media (max-width: 768px) {
          .hero {
            padding: 80px 0;
          }

          .hero-logo {
            margin-bottom: 40px;
          }

          .logo-svg {
            width: 50px;
            height: 50px;
          }

          .logo-text {
            font-size: 1.5rem;
          }

          .hero-title {
            font-size: 3rem;
            margin-bottom: 60px;
          }

          .tablet-device {
            width: 300px;
            height: 210px;
          }
        }

        @media (max-width: 480px) {
          .hero-logo {
            margin-bottom: 30px;
          }

          .logo-svg {
            width: 40px;
            height: 40px;
          }

          .logo-text {
            font-size: 1.25rem;
          }

          .hero-title {
            font-size: 2.5rem;
            margin-bottom: 40px;
          }

          .tablet-device {
            width: 250px;
            height: 175px;
          }
        }
      `}</style>
    </section>
  );
};

export default Hero;