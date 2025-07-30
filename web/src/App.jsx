import React from 'react';
import './App.css';
import Hero from './components/Hero.jsx';
import Terminal from './components/Terminal.jsx';
import Footer from './components/Footer.jsx';

function App() {
  return (
    <div className="App">
      <Hero />
      <Terminal />
      <Footer />
    </div>
  );
}

export default App;