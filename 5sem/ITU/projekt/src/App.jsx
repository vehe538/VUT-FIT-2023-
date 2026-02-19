
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import StartingPage from './components/StartingPage';
import TicTacToe1 from './components/TicTacToe1';
import TicTacToe2 from './components/TicTacToe2';
import './App.css';


function App() {
  return (
    <>
      <BrowserRouter>
        <Routes>
          <Route path="/" element={<StartingPage />} />
          <Route path="/tictactoe1p" element={<TicTacToe1 />}/>
          <Route path="/tictactoe2p" element={<TicTacToe2 />}/>
        </Routes>
      </BrowserRouter>

    </>
  );
}
export default App;
