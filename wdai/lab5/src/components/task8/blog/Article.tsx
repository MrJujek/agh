import React, { useEffect, useState } from 'react';
import { useParams, Link } from 'react-router-dom';
import './Blog.css';

interface Article {
    id: number;
    title: string;
    content: string;
}

const Article: React.FC = () => {
    const { id } = useParams<{ id: string }>();
    const [article, setArticle] = useState<Article | null>(null);

    useEffect(() => {
        const stored = localStorage.getItem('articles');
        if (stored && id) {
            const articles: Article[] = JSON.parse(stored);
            const found = articles.find(a => a.id === Number(id));
            if (found) {
                setArticle(found);
            }
        }
    }, [id]);

    if (!article) {
        return (
            <div className="blog-container" style={{ textAlign: 'center', marginTop: '4rem' }}>
                <h2>Article not found</h2>
                <Link to="/blog" className="blog-btn">Back to Blog</Link>
            </div>
        );
    }

    return (
        <div className="blog-container">
            <div className="blog-card">
                <h1 className="blog-title" style={{ fontSize: '3rem' }}>{article.title}</h1>
                <div style={{ lineHeight: '1.8', fontSize: '1.1rem', whiteSpace: 'pre-wrap' }}>
                    {article.content}
                </div>
            </div>
            <div style={{ marginTop: '2rem' }}>
                <Link to="/blog" className="blog-link"> &larr; Back to Blog</Link>
            </div>
        </div>
    );
};

export default Article;
