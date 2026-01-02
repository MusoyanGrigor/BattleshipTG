import { useEffect, useState } from "react";
import Board from "./components/Board";
import { getBoard, makeMove } from "./api/gameApi";

function getParamsFromURL() {
  const params = new URLSearchParams(window.location.search);
  return {
    gameID: params.get("game"),
    viewer: params.get("viewer"),
  };
}

export default function App() {
  const [board, setBoard] = useState(null);

  const { gameID, viewer } = getParamsFromURL();

  useEffect(() => {
    if (!gameID || !viewer) return;

    getBoard(gameID, viewer)
        .then(setBoard)
        .catch(console.error);
  }, [gameID, viewer]);

  async function handleMove(x, y) {
    if (!gameID || !viewer) return;

    const data = await makeMove(gameID, viewer, x, y);
    setBoard(data);
  }

  if (!gameID) return <p>No game ID provided.</p>;
  if (!viewer) return <p>No viewer provided.</p>;
  if (!board) return <p>Loading...</p>;

  return (
      <div>
          <Board board={board} onMove={handleMove} />
      </div>
  );
}
