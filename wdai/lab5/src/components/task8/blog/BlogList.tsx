import React, { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';
import './Blog.css';

interface Article {
    id: number;
    title: string;
    content: string;
}

const BlogList: React.FC = () => {
    const [articles, setArticles] = useState<Article[]>([]);

    useEffect(() => {
        const stored = localStorage.getItem('articles');
        if (stored) {
            try {
                setArticles(JSON.parse(stored));
            } catch (e) {
                console.error("Failed to parse articles", e);
            }
        }
    }, []);

    return (
        <div className="blog-container">
            <h2 className="blog-title">Articles</h2>

            <Link to="/dodaj" className="blog-btn" style={{ marginBottom: '2rem' }}>
                Add New Article
            </Link>

            <div style={{ display: 'flex', flexDirection: 'column', gap: '1rem' }}>
                {articles.length === 0 ? (
                    <p style={{ opacity: 0.6 }}>No articles found. Be the first to add one!</p>
                ) : (
                    articles.map(article => (
                        <div key={article.id} className="blog-card">
                            <h3 style={{ marginTop: 0 }}>
                                <Link to={`/article/${article.id}`} className="blog-link">
                                    {article.title}
                                </Link>
                            </h3>
                            <p style={{ opacity: 0.7, overflow: 'hidden', textOverflow: 'ellipsis', whiteSpace: 'nowrap' }}>
                                {article.content}
                            </p>
                        </div>
                    ))
                )}
            </div>
            <div style={{ marginTop: '2rem' }}>
                <Link to="/" className="blog-link"> &larr; Back to Home</Link>
            </div>
        </div>
    );
};

export default BlogList;
