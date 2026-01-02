import Cell from "./Cell.jsx";

export default function Grid({ cells, clickable, onMove, showIndexes }) {
    const size = cells.length;
    const IndexCell = ({ value }) => (
        <div
            className="cell empty"
            style={{
                display: "flex",
                alignItems: "center",
                justifyContent: "center",
                fontWeight: "bold",
                color: "#ff5a57",
            }}
        >
            {value}
        </div>
    );

    return (
        <div>
            {showIndexes && (
                <div style={{ display: "flex" }}>
                    <IndexCell value="" />
                    {Array.from({ length: size }, (_, i) => (
                        <IndexCell key={i} value={i} />
                    ))}
                </div>
            )}

            {cells.map((row, y) => (
                <div key={y} style={{ display: "flex" }}>
                    {showIndexes && <IndexCell value={y} />}
                    {row.map((cell, x) => (
                        <Cell
                            key={x}
                            value={cell}
                            onClick={
                                clickable && cell === "empty"
                                    ? () => onMove(x, y)
                                    : undefined
                            }
                        />
                    ))}
                </div>
            ))}
        </div>
    );
}