import React from 'react';
import Product from './Product';

const NewCart: React.FC = () => {
    const products = ["Apple", "Pear", "Banana", "Orange", "Grapes"];

    return (
        <div style={{
            padding: '2rem',
            backgroundColor: '#242424',
            borderRadius: '16px',
            border: '1px solid #333',
            maxWidth: '800px',
            margin: '0 auto',
            boxShadow: '0 20px 25px -5px rgba(0, 0, 0, 0.1), 0 10px 10px -5px rgba(0, 0, 0, 0.04)'
        }}>
            <h2 style={{
                color: '#fff',
                marginBottom: '1.5rem',
                textAlign: 'center',
                fontSize: '2rem',
                background: 'linear-gradient(to right, #646cff, #9089fc)',
                WebkitBackgroundClip: 'text',
                WebkitTextFillColor: 'transparent'
            }}>
                New Cart
            </h2>
            <div style={{
                display: 'flex',
                flexWrap: 'wrap',
                gap: '1.5rem',
                justifyContent: 'center'
            }}>
                {products.map((productName, index) => (
                    <Product key={index} nazwa={productName} />
                ))}
            </div>
        </div>
    );
};

export default NewCart;
