import { useEffect, useState } from "react";
import "../style/App.css";
import Bucket, { type IBucket } from "../components/Bucket";

const MAX_LEVEL = 10;

function Main() {
  const deleteContainer = (id: number) => {
    setContainers(
      (prev) => prev && prev.filter((container) => container.id != id)
    );
  };

  const [count, setCount] = useState(0);
  const [containers, setContainers] = useState<IBucket[]>();

  useEffect(() => {
    setContainers([
      {
        name: "1",
        id: 1,
        max_level: MAX_LEVEL,
        current_level: 5,
        deleteContainer,
        setContainers,
      },
      {
        name: "2",
        id: 2,
        max_level: MAX_LEVEL,
        current_level: 7,
        deleteContainer,
        setContainers,
      },
    ]);
  }, []);
  const [sum, setSum] = useState(0);
  const [name, setName] = useState("");

  const getID = () => {
    return containers
      ? containers.length
        ? containers[containers.length - 1].id + 1
        : 0
      : 0;
  };

  useEffect(() => {
    let temp = 0;
    if (!containers) return;
    containers.forEach((container) => {
      temp += Math.min(container.current_level, MAX_LEVEL);
    });
    // eslint-disable-next-line react-hooks/set-state-in-effect
    setSum(temp);
  }, [containers]);

  return (
    <>
      <div>
        <div>
          Nazwa pojemnika
          <input
            type="text"
            value={name}
            onChange={(e) => setName(e.target.value)}
          />
          <button
            onClick={async () => {
              await fetch("https://dummyjson.com/recipes")
                .then((res) => res.json())
                .then((data) => {
                  setName(
                    data.recipes[Math.floor(Math.random() * data.limit)].name
                  );
                })
                .catch((err) => {
                  console.error("Error fetching comments:", err);
                  setName("Losowy Pojemnik");
                });
            }}
          >
            Losuj
          </button>
        </div>
        <div>
          Początkowy stan
          <button onClick={() => setCount((prev) => prev - 1)}>-</button>
          {count}
          <button onClick={() => setCount((prev) => prev + 1)}>+</button>
        </div>
        <button
          disabled={
            count >= 0 && count <= MAX_LEVEL && name != "" ? false : true
          }
          onClick={() => {
            setContainers([
              ...(containers || []),
              {
                id: getID(),
                current_level: count,
                name,
                max_level: MAX_LEVEL,
                deleteContainer,
                setContainers,
              },
            ]);
          }}
        >
          Stwórz pojemnik
        </button>
        <button
          onClick={() => {
            setContainers((prev) =>
              prev
                ? prev.map((container) => ({
                    ...container,
                    current_level: container.current_level + 1,
                  }))
                : prev
            );
          }}
        >
          Dolewka
        </button>
        <p>Suma: {sum}</p>
      </div>
      <div className="containers">
        {containers &&
          containers.map((data, i) => {
            return (
              <Bucket
                key={i}
                {...data}
                deleteContainer={deleteContainer}
                setContainers={setContainers}
              ></Bucket>
            );
          })}
      </div>
    </>
  );
}

export default Main;
