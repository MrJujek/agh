import { BrowserRouter, Route, Routes } from "react-router-dom";
import Main from "./pages/Main";
import ShapeDetails from "./components/ShapeDetails";

function App() {
  return (
    <>
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<Main />} />
          <Route path="/shape/:id" element={<ShapeDetails />} />
        </Routes>
      </BrowserRouter>
    </>
  );
}

export default App;
