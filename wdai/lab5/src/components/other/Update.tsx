import React, { useState } from 'react';

const Update: React.FC = () => {
    const [product, setProduct] = useState({ name: "Tomato", price: 50 });

    const updatePrice = () => {
        setProduct(prev => ({
            ...prev,
            price: 100
        }));
    };

    return (
        <div style={{
            padding: '1.5rem',
            backgroundColor: 'rgba(255, 255, 255, 0.05)',
            borderRadius: '12px',
            border: '1px solid rgba(255, 255, 255, 0.1)',
            maxWidth: '400px',
            margin: '1rem auto',
            textAlign: 'center'
        }}>
            <h3 style={{ color: '#fff', margin: '0 0 1rem 0' }}>Update State</h3>

            <div style={{
                marginBottom: '1rem',
                fontSize: '1.1rem',
                color: '#e0e0e0'
            }}>
                Currently {product.name} costs {product.price}
            </div>

            <button
                onClick={updatePrice}
                style={{
                    backgroundColor: '#646cff',
                    color: 'white',
                    border: 'none',
                    padding: '0.6rem 1.2rem',
                    borderRadius: '8px',
                    fontSize: '1rem',
                    cursor: 'pointer',
                    transition: 'background-color 0.2s'
                }}
                onMouseOver={(e) => e.currentTarget.style.backgroundColor = '#535bf2'}
                onMouseOut={(e) => e.currentTarget.style.backgroundColor = '#646cff'}
            >
                Change price
            </button>
        </div>
    );
};

export default Update;
