import React from 'react';
import { Link } from 'react-router-dom';
import './Blog.css';

const Home: React.FC = () => {
    return (
        <div className="blog-container" style={{ textAlign: 'center', marginTop: '10vh' }}>
            <h1 className="blog-title">Welcome to the Blog App</h1>
            <p style={{ fontSize: '1.2rem', marginBottom: '2rem', opacity: 0.8 }}>
                Discover interesting articles or share your own thoughts.
            </p>

            <div style={{ display: 'flex', gap: '1rem', justifyContent: 'center' }}>
                <Link to="/blog" className="blog-btn">
                    Go to Blog
                </Link>
                <Link to="/licznik" className="blog-btn" style={{ backgroundColor: '#2c2c2c' }}>
                    Check Licznik (8.1)
                </Link>
                <Link to="/old-labs" className="blog-btn" style={{ backgroundColor: '#2c2c2c' }}>
                    Old Labs
                </Link>
            </div>
        </div>
    );
};

export default Home;
