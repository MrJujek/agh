let originalData = [];

const tableBody = document.getElementById("tableBody");
const filterInput = document.getElementById("filterInput");
const sortSelect = document.getElementById("sortSelect");

async function fetchData() {
  try {
    const response = await fetch("https://dummyjson.com/products");
    const data = await response.json();

    originalData = data.products.slice(0, 30);

    renderTable(originalData);
  } catch (error) {
    alert("Error: " + error.message);
  }
}

function renderTable(data) {
  tableBody.innerHTML = "";

  data.forEach((product) => {
    const row = document.createElement("tr");

    const imgCell = document.createElement("td");
    const img = document.createElement("img");
    img.src = product.thumbnail;
    img.alt = product.title;
    img.className = "product-img";
    imgCell.appendChild(img);

    const titleCell = document.createElement("td");
    titleCell.textContent = product.title;

    const descCell = document.createElement("td");
    descCell.textContent = product.description;

    row.appendChild(imgCell);
    row.appendChild(titleCell);
    row.appendChild(descCell);

    tableBody.appendChild(row);
  });
}

function updateTable() {
  let processedData = [...originalData];

  const phrase = filterInput.value.toLowerCase();
  if (phrase) {
    processedData = processedData.filter(
      (item) =>
        item.title.toLowerCase().includes(phrase) ||
        item.description.toLowerCase().includes(phrase)
    );
  }

  const sortMode = sortSelect.value;
  if (sortMode === "asc") {
    processedData.sort((a, b) => a.title.localeCompare(b.title));
  } else if (sortMode === "desc") {
    processedData.sort((a, b) => b.title.localeCompare(a.title));
  }
  renderTable(processedData);
}

filterInput.addEventListener("input", updateTable);
sortSelect.addEventListener("change", updateTable);

fetchData();
