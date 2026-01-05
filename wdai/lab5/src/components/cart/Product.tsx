import React from 'react';

interface ProduktProps {
  nazwa: string;
}

const Produkt: React.FC<ProduktProps> = ({ nazwa }) => {
  return (
    <div style={{
      padding: '1rem',
      margin: '0.5rem',
      backgroundColor: 'rgba(255, 255, 255, 0.05)',
      backdropFilter: 'blur(10px)',
      borderRadius: '12px',
      border: '1px solid rgba(255, 255, 255, 0.1)',
      color: '#e0e0e0',
      fontFamily: 'system-ui, -apple-system, sans-serif',
      boxShadow: '0 4px 6px rgba(0, 0, 0, 0.1)',
      display: 'flex',
      alignItems: 'center',
      justifyContent: 'center',
      width: '150px',
      transition: 'transform 0.2s ease',
      cursor: 'default'
    }}
      onMouseEnter={(e) => e.currentTarget.style.transform = 'translateY(-2px)'}
      onMouseLeave={(e) => e.currentTarget.style.transform = 'translateY(0)'}
    >
      <span style={{ fontSize: '1.1rem', fontWeight: 500 }}>{nazwa}</span>
    </div>
  );
};

export default Produkt;
