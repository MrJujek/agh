import NewCart from './components/cart/NewCart';
import Counter from './components/counters/Counter';
import NewCounter from './components/counters/NewCounter';
import Form from './components/forms/Form';
import Password from './components/forms/Password';
import Login from './components/forms/Login';
import Ternary from './components/other/Ternary';
import Update from './components/other/Update';
import Students from './components/students/Students';
import StudentManager from './components/students/StudentManager';
import './App.css';

function App() {
  return (
    <div style={{ minHeight: '100vh', padding: '2rem', boxSizing: 'border-box', display: 'flex', flexDirection: 'column', gap: '2rem' }}>
      <NewCart />
      <div style={{ display: 'flex', gap: '2rem', justifyContent: 'center', flexWrap: 'wrap' }}>
        <Counter />
        <NewCounter />
      </div>
      <div style={{ display: 'flex', gap: '2rem', justifyContent: 'center', flexWrap: 'wrap' }}>
        <Form />
        <Password />
        <Login />
      </div>
      <div style={{ display: 'flex', gap: '2rem', justifyContent: 'center', flexWrap: 'wrap' }}>
        <Ternary />
        <Update />
      </div>
      <div style={{ display: 'flex', gap: '2rem', justifyContent: 'center', flexWrap: 'wrap' }}>
        <Students />
        <StudentManager />
      </div>
    </div>
  );
}

export default App;
