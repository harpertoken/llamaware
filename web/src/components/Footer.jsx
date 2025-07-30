import React from 'react';
import { User, Github, Linkedin } from 'lucide-react';

const Footer = () => {
  return (
    <footer className="footer">
      <div className="container">
        <div className="footer-content">
          <div className="footer-top">
            <div className="footer-links">
              <a href="https://ingenuityapp.vercel.app/about.html" className="footer-link" target="_blank" rel="noopener noreferrer">About</a>
              <a href="https://startdownloadingenuitycli.netlify.app" className="footer-link" target="_blank" rel="noopener noreferrer">Download</a>
              <a href="https://ingenuityllms.vercel.app/pages/terms.html" className="footer-link" target="_blank" rel="noopener noreferrer">Terms</a>
              <a href="https://ingenuityllamaware.netlify.app" className="footer-link" target="_blank" rel="noopener noreferrer">Case Study</a>
            </div>
            <div className="social-links">
              <a href="https://github.com/ingenuityto" className="social-link" target="_blank" rel="noopener noreferrer" title="GitHub">
                <Github size={20} />
              </a>
              <a href="https://www.linkedin.com/company/ingenuitynet" className="social-link" target="_blank" rel="noopener noreferrer" title="LinkedIn">
                <Linkedin size={20} />
              </a>
            </div>
          </div>

          <div className="footer-bottom">
            <div className="footer-brand">
              <User size={16} className="brand-icon" />
              <span className="copyright">© Llamaware. 2025</span>
            </div>
            <div className="footer-legal">
              <span className="legal-text">All Rights Reserved</span>
            </div>
          </div>
        </div>
      </div>

      <style jsx>{`
        .footer {
          background: #ffffff;
          border-top: 1px solid #e2e8f0;
          padding: 24px 0;
          margin-top: 50px;
        }

        .footer-content {
          display: flex;
          flex-direction: column;
          gap: 20px;
        }

        .footer-top {
          display: flex;
          justify-content: space-between;
          align-items: center;
        }

        .footer-links {
          display: flex;
          gap: 24px;
        }

        .footer-link {
          color: #1a202c;
          text-decoration: none;
          font-size: 0.85rem;
          font-weight: 500;
          transition: color 0.2s ease;
        }

        .footer-link:hover {
          color: #38a169;
        }

        .social-links {
          display: flex;
          gap: 16px;
          align-items: center;
        }

        .social-link {
          color: #718096;
          text-decoration: none;
          display: flex;
          align-items: center;
          padding: 8px;
          border-radius: 6px;
          transition: all 0.2s ease;
        }

        .social-link:hover {
          color: #38a169;
          background-color: rgba(56, 161, 105, 0.1);
        }

        .footer-bottom {
          display: flex;
          justify-content: space-between;
          align-items: center;
          padding-top: 16px;
          border-top: 1px solid #e2e8f0;
        }

        .footer-brand {
          display: flex;
          align-items: center;
          gap: 6px;
        }

        .brand-icon {
          color: #1a202c;
        }

        .copyright {
          color: #718096;
          font-size: 0.8rem;
          font-weight: 400;
        }

        .footer-legal {
          display: flex;
          align-items: center;
        }

        .legal-text {
          color: #718096;
          font-size: 0.8rem;
          font-weight: 400;
        }

        @media (max-width: 768px) {
          .footer {
            padding: 20px 0;
          }

          .footer-links {
            gap: 16px;
          }

          .footer-bottom {
            flex-direction: column;
            gap: 10px;
            align-items: flex-start;
          }
        }

        @media (max-width: 480px) {
          .footer-links {
            gap: 12px;
          }

          .footer-link {
            font-size: 0.8rem;
          }
        }
      `}</style>
    </footer>
  );
};

export default Footer;