import Grid from "./Grid.jsx";

export default function Board({ board, onMove }) {
    if (!board) return null;

    const { myBoard, opponentBoard } = board;

    return (
        <div className="board-container">
            <div>
                <h3 className="board-title">My Board</h3>
                <Grid cells={myBoard.cells} clickable={false} showIndexes={true} />
            </div>

            <div>
                <h3 className="board-title">Opponent Board</h3>
                <Grid cells={opponentBoard.cells} clickable={true} onMove={onMove} showIndexes={true} />
            </div>
        </div>
    );
}
