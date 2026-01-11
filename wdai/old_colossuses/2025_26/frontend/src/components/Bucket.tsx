import { useEffect, useState } from "react";

export interface IBucket {
  current_level: number;
  max_level: number;
  name: string;
  id: number;
  deleteContainer: (id: number) => void;
  setContainers: React.Dispatch<React.SetStateAction<IBucket[] | undefined>>;
}

function Bucket(props: IBucket) {
  const { current_level, max_level, name, id, deleteContainer, setContainers } =
    props;
  const [curr, setCurr] = useState(current_level);

  useEffect(() => {
    setCurr(Math.min(Math.max(current_level, 0), max_level));
  }, [current_level, max_level]);

  useEffect(() => {
    setContainers((prev) =>
      prev
        ? prev.map((container) =>
            container.id === id
              ? { ...container, current_level: curr }
              : container
          )
        : prev
    );
  }, [curr, id, setContainers]);

  return (
    <div key={id}>
      <p>{name}</p>
      <button onClick={() => deleteContainer(id)}>Usuń</button>
      <div style={{ border: "1px solid white" }}>
        {[...Array(Math.max(max_level - curr, 0)).keys()].map((i) => {
          return <div key={i} className="level"></div>;
        })}
        {[...Array(curr).keys()].map((i) => {
          return (
            <div
              key={i}
              className="level"
              style={{ backgroundColor: "blue" }}
              onClick={() => {
                setCurr((prev) => (prev - 1 >= 0 ? prev - 1 : 0));
              }}
            >
              <img
                style={{
                  width: "10px",
                  height: "10px",
                  padding: "0px",
                  margin: "0px",
                }}
                src="/water.png"
                alt="woda"
              />
            </div>
          );
        })}
      </div>
    </div>
  );
}

export default Bucket;
