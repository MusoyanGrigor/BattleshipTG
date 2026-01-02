import "./Cell.css";

export default function Cell({ value, onClick }) {
    const className = `cell ${value || "empty"}`;

    return <div className={className} onClick={onClick} />;
}