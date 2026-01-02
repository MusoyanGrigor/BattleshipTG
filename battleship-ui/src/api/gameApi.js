export async function getBoard(gameID, viewer) {
    const res = await fetch(
        `http://localhost:18080/boards/${gameID}?viewer=${viewer}`
    );
    return res.json();
}

export async function makeMove(gameID, viewer, x, y) {
    await fetch(`http://localhost:18080/move/${gameID}?viewer=${viewer}`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ x, y })
    });
}
