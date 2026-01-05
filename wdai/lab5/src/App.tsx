import { BrowserRouter, Routes, Route } from 'react-router-dom';
import Home from './components/task8/blog/Home';
import BlogList from './components/task8/blog/BlogList';
import Article from './components/task8/blog/Article';
import AddArticle from './components/task8/blog/AddArticle';
import Licznik from './components/task8/Licznik';
import OldLabs from './OldLabs';
import './App.css'; // Importing App.css to keep some base styles if needed

function App() {
  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<Home />} />
        <Route path="/blog" element={<BlogList />} />
        <Route path="/article/:id" element={<Article />} />
        <Route path="/dodaj" element={<AddArticle />} />
        <Route path="/licznik" element={<Licznik />} />
        <Route path="/old-labs" element={<OldLabs />} />
      </Routes>
    </BrowserRouter>
  );
}

export default App;
